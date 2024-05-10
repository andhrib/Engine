#version 460 core

const int NUM_OF_LIGHTS = 3;

const int POINT_LIGHT = 0;
const int DIRECTIONAL_LIGHT = 1;

const float POINT_SHADOW_BIAS = 0.15;
const int SHADOW_SAMPLES = 20;
const vec3 SAMPLE_OFFSET_DIRECTIONS[SHADOW_SAMPLES] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

const float DIR_SHADOW_BIAS = 0.005;

struct Material 
{
	sampler2D texture_albedo;
	sampler2D texture_specular;
	sampler2D texture_normal;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float shininess;
};

in vec2 texCoords;
in vec3 fragPos;
in vec4 lightSpaceFragPos;
in vec3 tangentLightPositions[NUM_OF_LIGHTS];
in vec3 tangentViewPos;
in vec3 tangentFragPos;
in vec3 tangentDirLightDirection;

out vec4 fragColor;

uniform Material u_material;
uniform int u_lightingType;
uniform samplerCube u_pointShadowMap[NUM_OF_LIGHTS];
uniform float u_farPlane;
uniform sampler2D u_dirShadowMap;
uniform vec3 u_viewPos;
uniform vec3 u_lightPositions[NUM_OF_LIGHTS];

vec3 calcPointLight(vec3 normal, vec3 albedoColor, vec3 specularColor, vec3 viewDir);
vec3 calcDirLight(vec3 normal, vec3 albedoColor, vec3 specularColor, vec3 viewDir);
float calcPointShadow(int idx, float diskRadius);
float calcDirShadow();

void main() 
{
	vec3 albedoColor = texture(u_material.texture_albedo, texCoords).rgb;
	vec4 specularTexture = texture(u_material.texture_specular, texCoords);
	vec3 specularColor = vec3(specularTexture.r);
	vec3 normal = texture(u_material.texture_normal, texCoords).rgb;
	normal = normal * 2.0 - 1.0; // transform normal vector from [0, 1] to [-1, 1]
	// unit vector that points from the fragment to the camera
	vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
	vec3 lighting;

	switch (u_lightingType)
	{
		case POINT_LIGHT:
			vec3 pointLight = calcPointLight(normal, albedoColor, specularColor, viewDir);
			lighting = pointLight;
			break;

		case DIRECTIONAL_LIGHT:
			vec3 dirLight = calcDirLight(normal, albedoColor, specularColor, viewDir);
			float shadow = calcDirShadow();
			lighting = dirLight * (1.0 - shadow);
			break;
	}
	gl_FragColor = vec4(lighting, 1.0);
}

vec3 calcPointLight(vec3 normal, vec3 albedoColor, vec3 specularColor, vec3 viewDir) 
{
	vec3 pointLight = u_material.ambientStrength * albedoColor;
	float viewDistance = length(u_viewPos - fragPos);
	// disk radius is proportional to the distance from the light source, making the shadows sharper up close
	float diskRadius = (1.0 + (viewDistance / u_farPlane)) / 25.0;

	for (int i = 0; i < NUM_OF_LIGHTS; i++) 
	{
		// unit vector that points from the fragment to the light source
		vec3 lightDir = normalize(tangentLightPositions[i] - tangentFragPos);
		// vector that points in the direction of reflected light
		vec3 reflectDir = reflect(-lightDir, normal);
		// diffuse component
		float diff = max(dot(normal, lightDir), 0.0);
		diff *= u_material.diffuseStrength;
		// specular component
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
		spec *= u_material.specularStrength;
		// attenuation
		float distance = length(u_lightPositions[i] - fragPos);
		float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
		// shadow
		float shadow = calcPointShadow(i, diskRadius);
		// add the diffuse and specular components to the pointLight color
		pointLight += (diff * albedoColor + spec * specularColor) * attenuation * (1 - shadow);
	}

	return pointLight;
}

vec3 calcDirLight(vec3 normal, vec3 albedoColor, vec3 specularColor, vec3 viewDir) 
{
	vec3 dirLight = u_material.ambientStrength * albedoColor;

	// unit vector that points from the fragment to the light source
	vec3 lightDir = normalize(-tangentDirLightDirection);
	// vector that points in the direction of reflected light
	vec3 reflectDir = reflect(-lightDir, normal);
	// diffuse component
	float diff = max(dot(normal, lightDir), 0.0);
	diff *= u_material.diffuseStrength;
	// specular component
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	spec *= u_material.specularStrength;
	// add the diffuse and specular components to the dirLight color
	dirLight += (diff * albedoColor + spec * specularColor);

	return dirLight;
}

float calcPointShadow(int idx, float diskRadius)
{
	float shadow = 0.0;
	// get the vector that points from the fragment to the light source
	vec3 fragToLight = fragPos - u_lightPositions[idx];
	// the current depth value from the light's perspective
	float currentDepth = length(fragToLight);

	for (int i = 0; i < SHADOW_SAMPLES; i++)
	{
		// the closest depth value from the light's perspective
		float closestDepth = texture(u_pointShadowMap[idx], 
		fragToLight + SAMPLE_OFFSET_DIRECTIONS[i] * diskRadius).r;
		// convert closestDepth from range [0, 1] to [0, farPlane]
		closestDepth *= u_farPlane;
		// check whether the current fragment is in shadow
		if (currentDepth - POINT_SHADOW_BIAS > closestDepth)
		{
			shadow += 1.0;
		}
	}

	// average the shadow value
	shadow /= float(SHADOW_SAMPLES);

	return shadow;
}

float calcDirShadow() 
{
	float shadow = 0.0;
	// first convert the lightSpaceFragPos to range [-1, 1], then to [0, 1]
	vec3 projCoords = lightSpaceFragPos.xyz / lightSpaceFragPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	// the current depth value from the light's perspective
	float currentDepth = projCoords.z;
	// the closest depth value from the light's perspective
	float closestDepth = texture(u_dirShadowMap, projCoords.xy).r;
	vec2 texelSize = 1.0 / textureSize(u_dirShadowMap, 0);

	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(u_dirShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - DIR_SHADOW_BIAS > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	// average the shadow value
	shadow /= 9.0;

	return shadow;
}
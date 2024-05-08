#version 460 core

const int NUM_OF_LIGHTS = 3;

const int POINT_LIGHT = 0;
const int DIRECTIONAL_LIGHT = 1;

const float SHADOW_BIAS = 0.15;
const int SHADOW_SAMPLES = 20;
const float DISK_RADIUS = 0.05;
const vec3 SAMPLE_OFFSET_DIRECTIONS[SHADOW_SAMPLES] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
); 

struct Material 
{
	sampler2D texture_albedo;
	sampler2D texture_specular;

	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float shininess;
};

in vec3 normal;
in vec2 texCoords;
in vec3 fragPos;

out vec4 fragColor;

uniform Material u_material;
uniform vec3 u_lightPositions[NUM_OF_LIGHTS];
uniform vec3 u_cameraPos;
uniform vec3 u_dirLightDirection;
uniform int u_lightingType;
uniform samplerCube u_pointShadowMap[NUM_OF_LIGHTS];
uniform float u_farPlane;

vec3 calcPointLight(vec3 normal, vec3 albedoColor, vec3 specularColor, vec3 viewDir);
vec3 calcDirLight(vec3 normal, vec3 albedoColor, vec3 specularColor, vec3 viewDir);
float calcPointShadow(int idx, float diskRadius);

void main() 
{
	vec3 albedoColor = vec3(texture(u_material.texture_albedo, texCoords));
	vec4 specularTexture = texture(u_material.texture_specular, texCoords);
	vec3 specularColor = vec3(specularTexture.r);
	// unit vector that points from the fragment to the camera
	vec3 viewDir = normalize(u_cameraPos - fragPos);
	// normals are not necessarily normalised
	vec3 norm = normalize(normal);
	vec3 lighting;

	switch (u_lightingType)
	{
		case POINT_LIGHT:
			vec3 pointLight = calcPointLight(norm, albedoColor, specularColor, viewDir);
			lighting = pointLight;
			break;

		case DIRECTIONAL_LIGHT:
			vec3 dirLight = calcDirLight(norm, albedoColor, specularColor, viewDir);
			lighting = dirLight;
			break;
	}
	gl_FragColor = vec4(lighting, 1.0);
}

vec3 calcPointLight(vec3 normal, vec3 albedoColor, vec3 specularColor, vec3 viewDir) 
{
	vec3 pointLight = u_material.ambientStrength * albedoColor;
	float viewDistance = length(u_cameraPos - fragPos);
	// disk radius is proportional to the distance from the light source, making the shadows sharper up close
	float diskRadius = (1.0 + (viewDistance / u_farPlane)) / 25.0;

	for (int i = 0; i < NUM_OF_LIGHTS; i++) 
	{
		// unit vector that points from the fragment to the light source
		vec3 lightDir = normalize(u_lightPositions[i] - fragPos);
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
	vec3 lightDir = normalize(-u_dirLightDirection);
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
	float currentDepth = length(fragToLight);

	for (int i = 0; i < SHADOW_SAMPLES; i++)
	{
		// get the closest depth value from the light's perspective
		float closestDepth = texture(u_pointShadowMap[idx], 
		fragToLight + SAMPLE_OFFSET_DIRECTIONS[i] * diskRadius).r;
		// convert closestDepth from range [0, 1] to [0, farPlane]
		closestDepth *= u_farPlane;
		// check whether the current fragment is in shadow
		if (currentDepth - SHADOW_BIAS > closestDepth)
		{
			shadow += 1.0;
		}
	}

	// average the shadow value
	shadow /= float(SHADOW_SAMPLES);

	return shadow;
}
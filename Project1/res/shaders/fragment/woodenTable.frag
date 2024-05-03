#version 460 core

const int NUM_OF_LIGHTS = 2;

struct Material {
	sampler2D texture_albedo;
	sampler2D texture_specular;
};

in vec3 normal;
in vec2 texCoords;
in vec3 fragPos;

out vec4 fragColor;

uniform Material u_material;
uniform vec3 u_lightPositions[NUM_OF_LIGHTS];
uniform vec3 u_cameraPos;

vec3 calcPointLight(vec3 normal, vec3 albedoColor, vec3 specularColor, vec3 viewDir);

void main() 
{
	vec3 albedoColor = vec3(texture(u_material.texture_albedo, texCoords));
	vec4 specularTexture = texture(u_material.texture_specular, texCoords);
	vec3 specularColor = vec3(specularTexture);
	// unit vector that points from the fragment to the camera
	vec3 viewDir = normalize(u_cameraPos - fragPos);
	// normals are not necessarily normalised
	vec3 norm = normalize(normal);

	vec3 pointLight = calcPointLight(norm, albedoColor, specularColor, viewDir);

	fragColor = vec4(pointLight, 1.0);
}

vec3 calcPointLight(vec3 normal, vec3 albedoColor, vec3 specularColor, vec3 viewDir) 
{
	float ambientStrength = 0.05;
	float specularStrength = 0.3;
	float pointLightStrength = 10.0;
	vec3 pointLight = ambientStrength * albedoColor;

	for (int i = 0; i < NUM_OF_LIGHTS; i++) 
	{
		// unit vector that points from the fragment to the light source
		vec3 lightDir = normalize(u_lightPositions[i] - fragPos);
		// vector that points in the opposite direction of the light source
		vec3 reflectDir = reflect(-lightDir, normal);
		// diffuse component
		float diff = max(dot(normal, lightDir), 0.0);
		// specular component
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		spec *= specularStrength;
		// attenuation
		float distance = length(u_lightPositions[i] - fragPos);
		float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
		// add the diffuse and specular components to the pointLight color
		pointLight += (diff * albedoColor + spec * specularColor) * attenuation;
	}

	return pointLight * pointLightStrength;
}
#version 460 core

in vec3 fragPos;

out vec4 fragColor;

uniform vec3 u_cameraPos;
// we use an equirectangular map as the skybox
uniform sampler2D u_skyMap;

void main() 
{
	vec3 direction = normalize(fragPos - u_cameraPos);
	// convert direction vector to spherical coordinates, then map from ranges [-pi, pi] and [-pi/2, pi/2] to range [0, 1]
	vec2 uv;
	uv.x = atan(direction.z, direction.x) / (2.0 * 3.14159265) + 0.5;
	uv.y = asin(direction.y) / 3.14159265 + 0.5;
	vec3 color = texture(u_skyMap, uv).rgb;

	fragColor = vec4(color, 1.0);
}
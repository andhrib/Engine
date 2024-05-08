#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

out vec3 normal;
out vec2 texCoords;
out vec3 fragPos;
out vec4 lightSpaceFragPos;

uniform mat4 u_model, u_view, u_projection;
uniform mat4 u_lightSpaceMatrix;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
	normal = mat3(transpose(inverse(u_model))) * a_normal;
	texCoords = a_texCoords;
	fragPos = vec3(u_model * vec4(a_position, 1.0f));
	lightSpaceFragPos = u_lightSpaceMatrix * vec4(fragPos, 1.0);
}
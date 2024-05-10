#version 460 core

const int NUM_OF_LIGHTS = 3;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;
layout (location = 3) in vec3 a_tangent;

out vec2 texCoords;
out vec3 fragPos;
out vec4 lightSpaceFragPos;
out vec3 tangentLightPositions[NUM_OF_LIGHTS];
out vec3 tangentViewPos;
out vec3 tangentFragPos;
out vec3 tangentDirLightDirection;

uniform mat4 u_model, u_view, u_projection;
uniform mat4 u_lightSpaceMatrix;
uniform vec3 u_lightPositions[NUM_OF_LIGHTS];
uniform vec3 u_viewPos;
uniform vec3 u_dirLightDirection;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
	texCoords = a_texCoords;
	fragPos = vec3(u_model * vec4(a_position, 1.0f));
	lightSpaceFragPos = u_lightSpaceMatrix * vec4(fragPos, 1.0);

	// calculate TBN matrix
	vec3 T = normalize(vec3(u_model * vec4(a_tangent, 0.0)));
	vec3 N = normalize(vec3(u_model * vec4(a_normal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T, B, N));
	for (int i = 0; i < NUM_OF_LIGHTS; i++)
	{
		tangentLightPositions[i] = TBN * u_lightPositions[i];
	}
	tangentViewPos = TBN * u_viewPos;
	tangentFragPos = TBN * fragPos;
	tangentDirLightDirection = TBN * u_dirLightDirection;
}
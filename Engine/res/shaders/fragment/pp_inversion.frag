#version 460 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D u_colorTexture;

void main()
{
	vec4 color = texture(u_colorTexture, texCoords);
	fragColor = vec4((vec3(1.0f) - color.xyz), 1.0f);
}
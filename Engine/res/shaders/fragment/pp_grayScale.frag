#version 460 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D u_colorTexture;

void main()
{
	vec4 color = texture(u_colorTexture, texCoords);
	float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	fragColor = vec4(average, average, average, 1.0);
}
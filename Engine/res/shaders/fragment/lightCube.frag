#version 460 core

out vec4 FragColor;

uniform vec3 u_pointLightColor;

void main() 
{
	FragColor = vec4(u_pointLightColor, 1.0);
}
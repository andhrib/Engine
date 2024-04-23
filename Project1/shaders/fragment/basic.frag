#version 460 core

struct Material {
	sampler2D texture_albedo;
};

in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform Material u_material;

void main() 
{
	fragColor = texture(u_material.texture_albedo, texCoords);
}
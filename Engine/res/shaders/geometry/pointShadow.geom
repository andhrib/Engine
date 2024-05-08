#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 u_lightSpaceMatrices[6];

out vec4 fragPos; // FragPos from GS (output from vertex shader)

void main()
{
	for (int face = 0; face < 6; face++) 
	{
		gl_Layer = face; // built-in variable that specifies to which face we render.
		for (int i = 0; i < 3; i++) // for each vertex of the triangle
		{
			fragPos = gl_in[i].gl_Position;
			gl_Position = u_lightSpaceMatrices[face] * fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}
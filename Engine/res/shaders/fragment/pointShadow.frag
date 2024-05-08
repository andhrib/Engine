#version 460 core

in vec4 fragPos;

uniform vec3 u_lightPos;
uniform float u_farPlane;

void main() 
{
	// calculate the distance between the fragment and the light source
	float lightDistance = length(u_lightPos - fragPos.xyz);
	// map the distance to [0,1] range
	lightDistance /= u_farPlane;
	// write this as modified depth
	gl_FragDepth = lightDistance;
}
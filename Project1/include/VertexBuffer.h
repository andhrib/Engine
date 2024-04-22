#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

using std::vector;

class VertexBuffer
{
private:
	unsigned int VBO;
	vector<float> buffer;

public:
	VertexBuffer(vector<float>& b);
	VertexBuffer() {}
	void bind();
	unsigned int getVBO();
};


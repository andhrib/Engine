#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <vector>

class VertexBuffer
{
private:
	unsigned int VBO;
	std::vector<float> buffer;

public:
	VertexBuffer(std::vector<float>& b);
	~VertexBuffer();
	void bind();
	unsigned int getVBO();
};


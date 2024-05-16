#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <vector>

class VertexBuffer
{
private:
	unsigned int VBO;

public:
	VertexBuffer(std::vector<float>& bufferData);
	~VertexBuffer();
	void bind();
	unsigned int getVBO() const;
};


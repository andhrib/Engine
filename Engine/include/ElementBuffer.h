#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <vector>

class ElementBuffer
{
private:
	unsigned int EBO;

public:
	ElementBuffer(std::vector<unsigned int>& bufferData);
	~ElementBuffer();
	void bind();
	unsigned int getEBO() const;
};


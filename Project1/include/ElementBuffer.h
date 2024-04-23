#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class ElementBuffer
{
private:
	unsigned int EBO;
	std::vector<unsigned int> buffer;

public:
	ElementBuffer(std::vector<unsigned int>& b);
	ElementBuffer() {}
	void bind();
	unsigned int getEBO();
};


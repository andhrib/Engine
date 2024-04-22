#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

using std::vector;

class ElementBuffer
{
private:
	unsigned int EBO;
	vector<unsigned int> buffer;

public:
	ElementBuffer(vector<unsigned int>& b);
	ElementBuffer() {}
	void bind();
	unsigned int getEBO();
};


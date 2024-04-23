#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(std::vector<unsigned int>& b) : buffer(b)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(int), buffer.data(), GL_STATIC_DRAW);
}

void ElementBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

unsigned int ElementBuffer::getEBO()
{
	return EBO;
}

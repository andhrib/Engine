#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(std::vector<unsigned int>& bufferData)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferData.size() * sizeof(int), bufferData.data(), GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &EBO);
}

void ElementBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

unsigned int ElementBuffer::getEBO() const
{
	return EBO;
}

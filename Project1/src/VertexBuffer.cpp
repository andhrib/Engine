#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(vector<float>& b) : buffer(b)
{
	glGenBuffers(1, &VBO);
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer.data(), GL_STATIC_DRAW);
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

unsigned int VertexBuffer::getVBO()
{
	return VBO;
}

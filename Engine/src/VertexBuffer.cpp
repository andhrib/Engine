#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(std::vector<float>& bufferData)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), bufferData.data(), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &VBO);
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

unsigned int VertexBuffer::getVBO() const
{
	return VBO;
}

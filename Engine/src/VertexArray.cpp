#include "VertexArray.h"

VertexArray::VertexArray(std::vector<float>& vbData, std::vector<unsigned int>& ebData) : vb(vbData), eb(ebData)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	vb.bind();
	eb.bind();
	glBindVertexArray(0);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &VAO);
}

void VertexArray::setFormat(std::vector<unsigned int>& attributeLengths, unsigned int stride)
{
	glBindVertexArray(VAO);
	vb.bind();

	unsigned int offset = 0;

	for (unsigned int idx = 0; idx < attributeLengths.size(); idx++) {
		glVertexAttribPointer(idx, attributeLengths[idx], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(idx);
		offset += attributeLengths[idx];
	}

	glBindVertexArray(0);
}

void VertexArray::bind()
{
	glBindVertexArray(VAO);
}

void VertexArray::unBind()
{
	glBindVertexArray(0);
}

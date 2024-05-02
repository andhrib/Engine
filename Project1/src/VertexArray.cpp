#include "VertexArray.h"

VertexArray::VertexArray(std::vector<float>& vbData, std::vector<unsigned int>& ebData) : vb(vbData), eb(ebData)
{
	glGenVertexArrays(1, &VAO);
	this->bind();
	vb.bind();
	eb.bind();

	this->unBind();
}



VertexArray::~VertexArray()
{
	//glDeleteVertexArrays(1, &VAO);
}

void VertexArray::setFormat(std::vector<unsigned int>& attributeLengths, unsigned int stride)
{
	this->bind();
	vb.bind();

	unsigned int offset = 0;

	for (unsigned int idx = 0; idx < attributeLengths.size(); idx++) {
		glVertexAttribPointer(idx, attributeLengths[idx], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(idx);
		offset += attributeLengths[idx];
	}

	this->unBind();
}

void VertexArray::bind()
{
	glBindVertexArray(VAO);
}

void VertexArray::unBind()
{
	glBindVertexArray(0);
}

void VertexArray::instancingMat4(const std::vector<glm::mat4>& data)
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * data.size(), &(data.data())[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

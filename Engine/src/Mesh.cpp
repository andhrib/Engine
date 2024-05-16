#include "Mesh.h"

Mesh::Mesh(std::vector<float>& vbData, std::vector<unsigned int>& ebData) :
	va(vbData, ebData), numIndices(ebData.size())
{
	std::vector<unsigned int> attributeLengths = { 3, 3, 2, 3 };
	unsigned int stride = 11;
	va.setFormat(attributeLengths, stride);
}

void Mesh::draw()
{
	// Draw mesh
	va.bind();
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

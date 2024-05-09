#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "ElementBuffer.h"
#include "VertexBuffer.h"

class VertexArray
{
private:
	unsigned int VAO;
	ElementBuffer eb;
	VertexBuffer vb;

public:
	VertexArray(std::vector<float>& vbData, std::vector<unsigned int>& ebData);
	~VertexArray();
	// set the format of the attributes in the vertex array
	void setFormat(std::vector<unsigned int>& attributeLengths, unsigned int stride);
	void bind();
	void unBind();
	void instancingMat4(const std::vector<glm::mat4>& data);
};


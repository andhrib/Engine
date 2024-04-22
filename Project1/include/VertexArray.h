#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "ElementBuffer.h"
#include "VertexBuffer.h"

using std::vector;

class VertexArray
{
private:
	unsigned int VAO;
	ElementBuffer eb;
	VertexBuffer vb;

public:
	VertexArray(vector<float>& vbData, vector<unsigned int>& ebData);
	VertexArray(vector<float>& vbData);
	VertexArray() {}
	void bind();
	void unBind();
	void instancingMat4(const std::vector<glm::mat4>& data);
};


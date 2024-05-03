#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArray.h"
#include "Shader.h"

#include <vector>

class Skybox {
private:
	VertexArray va;
	Shader shader;

	// vertex buffer data
	std::vector<float> vbData;
	// element buffer data
	std::vector<unsigned int> ebData;

public:
	Skybox();
	void draw(glm::mat4& view, glm::mat4& projection);
};
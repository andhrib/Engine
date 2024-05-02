#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArray.h"
#include "Shader.h"

#include <vector>

class LightCube {
private:
	VertexArray va;
	// a simple shader that sets the color to white
	Shader shader;
	std::vector<glm::vec3> positions; // multiple positions to enable having multiple light cubes

	// vertex buffer data
	std::vector<float> vbData;
	// element buffer data
	std::vector<unsigned int> ebData;

public:
	LightCube(std::vector<glm::vec3>& pos);
	void draw(glm::mat4& view, glm::mat4& projection);
};
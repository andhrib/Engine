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
	// multiple positions to enable having multiple light cubes
	std::vector<glm::vec3> positions;

public:
	LightCube(std::vector<glm::vec3>& pos);
	void draw(glm::mat4& view, glm::mat4& projection);
};
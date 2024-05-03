#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Shader.h"
#include "Model.h"

class WoodenTable
{
private:
	Model model;
	Shader shader;

	// we keep the model matrix stored in between frames for rotation purposes
	glm::mat4 modelMat;

public:
	// the rotation axis
	glm::vec3 axis;
	// the rotation speed
	float rotSpeed;

public:
	WoodenTable(std::vector<glm::vec3>& lightCubePositions, glm::vec3 cameraPos);
	void draw(glm::mat4& view, glm::mat4& projection, float deltaTime);

private:
	// set the light positions in the shader
	void setLightPositions(std::vector<glm::vec3>& lightCubePositions);
};


#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	WoodenTable();
	void draw(glm::mat4& view, glm::mat4& projection, float deltaTime);
};


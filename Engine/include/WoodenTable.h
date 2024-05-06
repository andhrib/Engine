#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Shader.h"
#include "Model.h"

enum LightingType
{
	POINT_LIGHT = 0,
	DIRECTIONAL_LIGHT = 1
};

enum MaterialType
{
	WOOD = 0,
	METAL = 1,
	ROCK = 2
};

class WoodenTable
{
private:
	Model model;
	Shader shader;

	// the model matrix is kept stored in between frames for rotation purposes
	glm::mat4 rotationMat;

public:
	// the rotation axis
	glm::vec3 axis;
	// the rotation speed
	float rotSpeed;
	// the position of the wooden table
	glm::vec3 position;
	// the direction of the directional light
	glm::vec3 dirLightDirection;
	// the lighting type
	LightingType lightingType;

public:
	WoodenTable(std::vector<glm::vec3>& lightCubePositions, glm::vec3 cameraPos);
	void draw(glm::mat4& view, glm::mat4& projection, float deltaTime);
	void setLightingType(LightingType lt);
	void setMaterial(MaterialType ct);

private:
	// set the light positions in the shader
	void setLightPositions(std::vector<glm::vec3>& lightCubePositions);
};


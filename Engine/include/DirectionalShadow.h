#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Shader.h"

const unsigned int DIR_SHADOW_WIDTH = 1024;
const unsigned int DIR_SHADOW_HEIGHT = 1024;
const float DIR_SHADOW_NEAR = 0.1f;
const float DIR_SHADOW_FAR = 30.0f;

class DirectionalShadow
{
private:
	unsigned int depthMapFBO;
	unsigned int depthMap;
	Shader shader;

public:
	// the light space matrix
	glm::mat4 lightSpaceMatrix;

public:
	DirectionalShadow(glm::vec3& dirLightDirection);
	void configureShader();

	// getters
	Shader& getShader();
	unsigned int getDepthMap();
};
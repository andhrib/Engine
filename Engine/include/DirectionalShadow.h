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
public:
	// the shader used to render the scene to the depth map
	Shader shader;

private:
	// the view matrix
	glm::mat4 view;
	// the projection matrix
	glm::mat4 projection;
	// the light space matrix
	glm::mat4 lightSpaceMatrix;

	// the depth map frame buffer object
	unsigned int depthMapFBO;
	// the depth map texture
	unsigned int depthMap;

public:
	DirectionalShadow(glm::vec3& dirLightDirection);
	~DirectionalShadow();
	void configureShader();

	// setters
	void setDirLightDirection(glm::vec3& dirLightDirection);

	// getters
	unsigned int getDepthMap() const;
	const glm::mat4& getLightSpaceMatrix() const;
};
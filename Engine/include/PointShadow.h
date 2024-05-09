#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Shader.h"

const unsigned int POINT_SHADOW_WIDTH = 1024;
const unsigned int POINT_SHADOW_HEIGHT = 1024;
const float POINT_SHADOW_NEAR = 0.5f;
const float POINT_SHADOW_FAR = 15.0f;

class PointShadow
{
private:
	// the depth cubemap textures
	std::vector<unsigned int> depthCubemaps;
	// the depth map frame buffer objects
	std::vector<unsigned int> depthMapFBOs;
	// the light space matrices, six for each light cube (one per face)
	std::vector<std::vector<glm::mat4>> lightSpaceMatrices;
	// the positions of the light cubes
	std::vector<glm::vec3>& lightCubePositions;
	// the directions at which the light cube faces are 'looking'
	std::vector<glm::vec3> lightCubeFaceDirections;
	// the up vectors of the light cube faces, used to calculate the view matrices for each face
	std::vector<glm::vec3> lightCubeFaceUpVectors;
	// the shader used to render the scene to the depth cubemap
	Shader shader;

public:
	PointShadow(std::vector<glm::vec3>& lightCubePositions);
	void configureShader(int idx);
	Shader& getShader();
	std::vector<unsigned int>& getDepthCubemaps();

private:
	void setupShadows(std::vector<glm::vec3>& lightCubePositions);
};
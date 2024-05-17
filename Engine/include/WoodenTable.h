#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "Shader.h"
#include "Model.h"
#include "PointShadow.h"

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

	// the rotation matrix and translation matrix, used to calculate the model matrix
	glm::mat4 rotationMat;
	glm::mat4 translationMat;
	glm::mat4 modelMat;

	// the lighting type
	LightingType lightingType;
	// the material type
	MaterialType materialType;
public:
	// the rotation axis
	glm::vec3 axis;
	// the rotation speed
	float rotSpeed;
	// the position of the wooden table
	glm::vec3 position;

public:
	WoodenTable(std::vector<glm::vec3>& lightCubePositions, const glm::vec3& cameraPos, glm::vec3& dirLightDirection);
	void draw(glm::mat4& view, glm::mat4& projection);
	void drawShadow(Shader& shadowShader);
	void updateModelMatrix(float deltaTime);
	// setters
	void setLighting(LightingType mt);
	void setMaterial(MaterialType ct);
	void setPointShadowMaps(const std::vector<unsigned int>& depthCubemaps);
	void setDirShadowMap(unsigned int depthMap);
	void setLightSpaceMatrix(const glm::mat4& lightSpaceMatrix);
	void setDirLightDirection(glm::vec3& dirLightDirection);
	void setDirLightColor(glm::vec3& dirLightColor);
	void setPointLightColor(glm::vec3& pointLightColor);
	// getters
	LightingType getLightingType() const;

private:
	// set the light positions in the shader
	void setLightPositions(std::vector<glm::vec3>& lightCubePositions);
};


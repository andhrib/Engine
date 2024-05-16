#include "DirectionalShadow.h"

DirectionalShadow::DirectionalShadow(glm::vec3& dirLightDirection) :
shader("res/shaders/vertex/directionalShadow.vert", "res/shaders/fragment/directionalShadow.frag")
{
	// generate the depth map
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// generate the depth map FBO
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// set the light space matrix
	view = glm::lookAt(-dirLightDirection, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, DIR_SHADOW_NEAR, DIR_SHADOW_FAR);
	lightSpaceMatrix = projection * view;
	shader.use();
	shader.setMat4("u_lightSpaceMatrix", lightSpaceMatrix);
}

DirectionalShadow::~DirectionalShadow()
{
	glDeleteFramebuffers(1, &depthMapFBO);
}

void DirectionalShadow::configureShader()
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void DirectionalShadow::setDirLightDirection(glm::vec3& dirLightDirection)
{
	view = glm::lookAt(-dirLightDirection, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = projection * view;
	shader.use();
	shader.setMat4("u_lightSpaceMatrix", lightSpaceMatrix);
}

unsigned int DirectionalShadow::getDepthMap() const
{
	return depthMap;
}

const glm::mat4& DirectionalShadow::getLightSpaceMatrix() const
{
	return lightSpaceMatrix;
}

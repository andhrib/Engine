#include "PointShadow.h"

PointShadow::PointShadow(std::vector<glm::vec3>& lightCubePositions) :
	lightCubePositions(lightCubePositions),
	shader("res/shaders/vertex/pointShadow.vert", "res/shaders/fragment/pointShadow.frag", "res/shaders/geometry/pointShadow.geom")
{
	lightCubeFaceDirections =
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	};
	lightCubeFaceUpVectors =
	{
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f)
	};

	setupShadows(lightCubePositions);
}

PointShadow::~PointShadow()
{
	for (unsigned int depthMapFBO : depthMapFBOs)
	{
		glDeleteFramebuffers(1, &depthMapFBO);
	}
}

const std::vector<unsigned int>& PointShadow::getDepthCubemaps() const
{
	return depthCubemaps;
}

void PointShadow::configureShader(int idx)
{
	// bind the depth map FBO
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOs[idx]);
	glClear(GL_DEPTH_BUFFER_BIT);
	shader.use();
	for (int i = 0; i < 6; i++)
	{
		shader.setMat4("u_lightSpaceMatrices[" + std::to_string(i) + "]", lightSpaceMatrices[idx][i]);
	}
	shader.setFloat("u_farPlane", POINT_SHADOW_FAR);
	shader.setVec3("u_lightPos", lightCubePositions[idx]);
}

void PointShadow::setupShadows(std::vector<glm::vec3>& lightCubePositions)
{
	// parameters for the light space matrices
	float aspect = (float)POINT_SHADOW_WIDTH / (float)POINT_SHADOW_HEIGHT;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, POINT_SHADOW_NEAR, POINT_SHADOW_FAR);
	// create the depth cube maps
	for (int i = 0; i < lightCubePositions.size(); i++)
	{
		// first create the depth cubemap
		unsigned int depthCubemap;
		glGenTextures(1, &depthCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		for (int j = 0; j < 6; j++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT, 
				POINT_SHADOW_WIDTH, POINT_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		depthCubemaps.push_back(depthCubemap);

		// then create the FBO
		unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		depthMapFBOs.push_back(depthMapFBO);

		// create the light space matrices
		lightSpaceMatrices.push_back(std::vector<glm::mat4>());
		for (int j = 0; j < 6; j++)
		{
			lightSpaceMatrices.back().push_back(shadowProj *
				glm::lookAt(lightCubePositions[i],
					lightCubePositions[i] + lightCubeFaceDirections[j],
					lightCubeFaceUpVectors[j]));
		}
	}
}

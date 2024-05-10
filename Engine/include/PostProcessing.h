#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VertexArray.h"

#include <vector>
#include <string>
#include <iostream>

const unsigned int SAMPLES = 4;

enum PostProcessingType
{
	NONE = 0,
	INVERSION = 1,
	GRAYSCALE = 2,
	SHARPEN = 3,
	BLUR = 4,
	EDGE_DETECTION = 5
};

class PostProcessing
{
private:
	std::vector<Shader> shaders;
	VertexArray va;
	// Framebuffer object with multisampling enabled
	unsigned int msFBO;
	// Texture color buffer with multisampling enabled
	unsigned int msTextureColorBuffer;
	// Renderbuffer object for depth and stencil attachments with multisampling enabled
	unsigned int msRBODepthStencil;
	// Framebuffer object with multisampling disabled for post-processing
	unsigned int FBO;
	// Texture color buffer with multisampling disabled for post-processing
	unsigned int textureColorBuffer;

public:
	PostProcessingType type;

public: 
	PostProcessing(int width, int height);
	~PostProcessing();
	void bindFramebuffer();
	void draw(int width, int height);
};
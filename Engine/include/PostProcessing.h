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
	// Framebuffer object
	unsigned int fbo;
	// Texture color buffer
	unsigned int textureColorBuffer;
	// Renderbuffer object for depth and stencil attachments
	unsigned int rboDepthStencil;

public:
	PostProcessingType type;

public: 
	PostProcessing(int width, int height);
	~PostProcessing();
	void bindFramebuffer();
	void draw();
};
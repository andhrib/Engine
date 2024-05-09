#include "PostProcessing.h"

static std::vector<float> vbData =
{
	-1.0f,  1.0f, 0.0f, 1.0f, // top left
	-1.0f, -1.0f, 0.0f, 0.0f, // bottom left
	 1.0f, -1.0f, 1.0f, 0.0f, // bottom right
	 1.0f,  1.0f, 1.0f, 1.0f  // top right
};

static std::vector<unsigned int> ebData =
{
	0, 1, 2,
	2, 3, 0
};

static std::string vertexShaderPath = "res/shaders/vertex/postProcessing.vert";
static std::vector<std::string> fragmentShaderPaths = 
{
	"res/shaders/fragment/pp_none.frag",
	"res/shaders/fragment/pp_inversion.frag",
	"res/shaders/fragment/pp_grayscale.frag",
	"res/shaders/fragment/pp_sharpen.frag",
	"res/shaders/fragment/pp_blur.frag",
	"res/shaders/fragment/pp_edgeDetection.frag"
};

PostProcessing::PostProcessing(int width, int height) : va(vbData, ebData), type(NONE)
{
	// generate and bind the framebuffer object
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// generate and bind the texture color buffer
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	// allocate memory for the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	// generate and bind the renderbuffer object for depth and stencil attachments
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	// allocate memory for the renderbuffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// attach it to currently bound framebuffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

	// check if the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// load the shaders
	shaders.reserve(fragmentShaderPaths.size());
	for (std::string& fragmentShaderPath : fragmentShaderPaths)
	{
		shaders.emplace_back(vertexShaderPath, fragmentShaderPath);
	}

	// set the vertex array format
	std::vector<unsigned int> attributeLengths = { 2, 2 };
	va.setFormat(attributeLengths, 4);
}

PostProcessing::~PostProcessing()
{
	// delete the framebuffer object
	glDeleteFramebuffers(1, &fbo);
	// delete the texture color buffer
	glDeleteTextures(1, &textureColorBuffer);
	// delete the renderbuffer object for depth and stencil attachments
	glDeleteRenderbuffers(1, &rboDepthStencil);
}

void PostProcessing::bindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void PostProcessing::draw()
{
	// bind the default framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// clear the color buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// disable depth testing
	glDisable(GL_DEPTH_TEST);

	// draw the quad that covers the entire screen
	va.bind();
	shaders[(int)type].use();
	shaders[(int)type].setInt("u_colorTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
}

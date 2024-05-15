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

PostProcessing::PostProcessing(int width, int height) : va(vbData, ebData), type(NONE),
windowWidth(width), windowHeight(height)
{
	glGenFramebuffers(1, &msFBO);
	glGenTextures(1, &msTextureColorBuffer);
	glGenRenderbuffers(1, &msRBODepthStencil);
	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &textureColorBuffer);
	setupFramebuffers();

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

void PostProcessing::bindFramebuffer(int width, int height)
{
	if (width != windowWidth || height != windowHeight) {
		windowWidth = width;
		windowHeight = height;
		setupFramebuffers();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, msFBO);
}

void PostProcessing::draw()
{
	// resolve the multisampled framebuffer to the normal intermediary framebuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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

void PostProcessing::setupFramebuffers()
{
	// prepare the framebuffer with multisampling enabled
	// generate and bind the framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, msFBO);

	// generate and bind the texture color buffer
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msTextureColorBuffer);
	// allocate memory for the texture
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLES, GL_RGB, windowWidth, windowHeight, GL_TRUE);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msTextureColorBuffer, 0);

	// generate and bind the renderbuffer object for depth and stencil attachments
	
	glBindRenderbuffer(GL_RENDERBUFFER, msRBODepthStencil);
	// allocate memory for the renderbuffer
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// attach it to currently bound framebuffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msRBODepthStencil);

	// check if the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// prepare the framebuffer with multisampling disabled for post-processing
	// generate and bind the framebuffer object
	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// bind the texture color buffer
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	// allocate memory for the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
}

PostProcessing::~PostProcessing()
{
	// delete the framebuffer object
	glDeleteFramebuffers(1, &msFBO);
	// delete the texture color buffer
	glDeleteTextures(1, &msTextureColorBuffer);
	// delete the renderbuffer object for depth and stencil attachments
	glDeleteRenderbuffers(1, &msRBODepthStencil);
}

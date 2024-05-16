#include "Skybox.h"

// vertex buffer data
static std::vector<float> vbData =
{                        // vertex positions
	-0.5f,  0.5f,  0.5f, // front top left
	-0.5f, -0.5f,  0.5f, // front bottom left
	 0.5f, -0.5f,  0.5f, // front bottom right
	 0.5f,  0.5f,  0.5f, // front top right
	-0.5f,  0.5f, -0.5f, // back top left
	-0.5f, -0.5f, -0.5f, // back bottom left
	 0.5f, -0.5f, -0.5f, // back bottom right
	 0.5f,  0.5f, -0.5f  // back top right
};

// element buffer data
static std::vector<unsigned int> ebData =
{					  // faces:
	0, 1, 2, 2, 3, 0, // front
	4, 0, 3, 3, 7, 4, // top
	4, 5, 1, 1, 0, 4, // left
	5, 6, 2, 2, 1, 5, // bottom
	3, 2, 6, 6, 7, 3, // right
	7, 6, 5, 5, 4, 7  // back
};

Skybox::Skybox() : shader("res/shaders/vertex/skybox.vert", "res/shaders/fragment/skybox.frag"),
va(vbData, ebData)
{
	// create and set up the vertex array
	std::vector<unsigned int> attributeLengths = { 3 }; // one attribute (position) with three coordinates
	va.setFormat(attributeLengths, 3);

	// load the skybox texture and flip it vertically
	shader.use();
	stbi_set_flip_vertically_on_load(true);
	shader.addTexture("res/skybox_cubemaps/photo_studio.hdr", "photo_studio");
	stbi_set_flip_vertically_on_load(false);
	// set the texture uniform
	shader.addTextureUniform("u_skyMap", 0);
	// set the default material to photo studio
	shader.changeMaterial("u_skyMap", "photo_studio");
}


void Skybox::draw(glm::mat4& view, glm::mat4& projection)
{
	// disable face culling for the skybox
	glDisable(GL_CULL_FACE);
	// modify the depth function to draw the skybox
	glDepthFunc(GL_LEQUAL);
	// bind the shader and vertex array
	shader.use();
	shader.setActiveTextures();
	va.bind();
	// setting the uniforms
	// the skybox should not be affected by translation, so we need to modify the view matrix
	glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
	shader.setMat4("u_view", viewNoTranslation);
	shader.setMat4("u_projection", projection);
	// draw
	glDrawElements(GL_TRIANGLES, ebData.size(), GL_UNSIGNED_INT, 0);
	// re-enable face culling
	glEnable(GL_CULL_FACE);
	// reset the depth function to default
	glDepthFunc(GL_LESS);
}

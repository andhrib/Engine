#include "LightCube.h"

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

// element buffer data, counterclockwise to enable face culling
static std::vector<unsigned int> ebData =
{					  // faces:
	0, 1, 2, 2, 3, 0, // front
	4, 0, 3, 3, 7, 4, // top
	4, 5, 1, 1, 0, 4, // left
	5, 6, 2, 2, 1, 5, // bottom
	3, 2, 6, 6, 7, 3, // right
	7, 6, 5, 5, 4, 7  // back
};

LightCube::LightCube(std::vector<glm::vec3>& pos) : positions(pos), 
shader("res/shaders/vertex/lightCube.vert", "res/shaders/fragment/lightCube.frag"),
va(vbData, ebData)
{
	// set up the vertex array
	std::vector<unsigned int> attributeLengths = {3}; // one attribute (position) with three coordinates
	va.setFormat(attributeLengths, 3);
}

void LightCube::draw(glm::mat4& view, glm::mat4& projection)
{
	va.bind();
	shader.use();
	shader.setActiveTextures();
	// setting the uniforms
	shader.setMat4("u_view", view);
	shader.setMat4("u_projection", projection);
	for (glm::vec3& pos : positions) {
		// different position for each light -> different model matrix
		glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
		model = glm::scale(model, glm::vec3(0.5f));
		shader.setMat4("u_model", model);

		// draw
		glDrawElements(GL_TRIANGLES, ebData.size(), GL_UNSIGNED_INT, 0);
	}
}

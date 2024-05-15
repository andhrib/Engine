#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include <Shader.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
    glm::vec3 Tangent;
};

class Mesh
{
private:
    //  render data
    unsigned int VAO, VBO, EBO;

public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

private:
	void setupMesh();

public:
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
    void draw(Shader &shader);
};


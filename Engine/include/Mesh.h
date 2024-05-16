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

#include "VertexArray.h"


class Mesh
{
private:
	VertexArray va;
	int numIndices;

public:
    Mesh(std::vector<float>& vbData, std::vector<unsigned int>& ebData);
    void draw();
};


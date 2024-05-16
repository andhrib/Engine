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
#include <iostream>

#include <Mesh.h>

class Model
{
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

public:
    Model(const std::string& path);
    void draw();

private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<float>& vbData, std::vector<unsigned int>& ebData);
};


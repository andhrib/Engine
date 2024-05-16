#include "Model.h"

Model::Model(const std::string& path)
{
	loadModel(path);
}

void Model::draw()
{
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].draw();
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    meshes.reserve(meshes.capacity() + node->mNumMeshes);
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::vector<float> vbData;
		std::vector<unsigned int> ebData;
		processMesh(mesh, scene, vbData, ebData);
        meshes.emplace_back(vbData, ebData);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<float>& vbData, std::vector<unsigned int>& ebData)
{
	// reserve space for each vertex: position (3), normal (3), texCoords (2), tangent (3)
	vbData.reserve(mesh->mNumVertices * 11);

    // read and store the vertex data from the mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        

        // position
        vbData.push_back(mesh->mVertices[i].x);
		vbData.push_back(mesh->mVertices[i].y);
		vbData.push_back(mesh->mVertices[i].z);

        // normal
        vbData.push_back(mesh->mNormals[i].x);
        vbData.push_back(mesh->mNormals[i].y);
        vbData.push_back(mesh->mNormals[i].z);

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
			vbData.push_back(mesh->mTextureCoords[0][i].x);
			vbData.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			vbData.push_back(0.0f);
			vbData.push_back(0.0f);
        }

		// tangent
		vbData.push_back(mesh->mTangents[i].x);
		vbData.push_back(mesh->mTangents[i].y);
		vbData.push_back(mesh->mTangents[i].z);
    }

    // read and store the indices data from the mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
		ebData.reserve(ebData.capacity() + face.mNumIndices);
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            ebData.push_back(face.mIndices[j]);
        }
    }
}

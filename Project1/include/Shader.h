#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stbi/stb_image.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using std::string;
using std::cout;

class Shader
{
private:
	unsigned int shaderProgram;
	std::vector<unsigned int> textures;

	unsigned int loadTexture(char const* path);

public:
	Shader(const string& vertexPath, const string& fragmentPath, const string& texturePath);
	Shader() {}
	void use() const;
	void addTexture(const string& path);

	// uniform setting functions
	void setMat4(const string& name, const glm::mat4& value) const;
	void setVec3(const string& name, const glm::vec3& value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
};


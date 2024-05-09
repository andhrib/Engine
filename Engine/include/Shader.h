#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stbi/stb_image.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

class Shader
{
private:
	unsigned int shaderProgram;
	// maps texture names to their id
	std::unordered_map<std::string, unsigned int> textureMap;
	// maps uniform names to their location
	std::unordered_map<std::string, int> uniformMap;
	// maps uniform names to the texture is sent to it
	std::unordered_map<std::string, std::string> configuration;

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
	~Shader();
	// use the shader program
	void use() const;
	// set all textures to active
	void setActiveTextures();
	// add a texture to the shader program
	void addTexture(const std::string& path, const std::string& texture_name);
	void addTextureUniform(const std::string& uniform_name, int location);
	void changeMaterial(const std::string& uniform_name, const std::string& texture_name);

	// uniform setting functions
	void setMat4(const std::string& name, const glm::mat4& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	// getter functions
	int getTextureCount() const;

private:
	// load the texture from the path
	unsigned int loadTexture(char const* path);
	// compile the shader from the path
	unsigned int compileShader(const std::string& path, GLenum type);
};


#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& texturePath) : textures()
{
	unsigned int vertexShader, fragmentShader;

	// retrieve the shader code from the shader files
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// vertex shader
	// link the shader file with the program and compile	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// error checking
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR_VERTEX_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	// link the shader file with the program and compile	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// error checking
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR_FRAGMENT_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// compile shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// error checking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR_SHADER_PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// add the base texture
	addTexture(texturePath);
}

void Shader::use() const
{
	glUseProgram(shaderProgram);
	for (unsigned int i = 0; i < (unsigned int) textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
}

void Shader::addTexture(const std::string& path)
{
	unsigned int textureID = loadTexture(path.c_str());
	textures.push_back(textureID);
}

unsigned int Shader::loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	int uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
	if (uniformLocation == -1) {
		std::cout << "FAILED_TO_GET_LOCATION_OF_UNIFORM: " << name << std::endl;
	}
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	int uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
	if (uniformLocation == -1) {
		std::cout << "FAILED_TO_GET_LOCATION_OF_UNIFORM: " << name << std::endl;
	}
	glUniform3f(uniformLocation, value.x, value.y, value.z);
}

void Shader::setInt(const std::string& name, int value) const
{
	int uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
	if (uniformLocation == -1) {
		std::cout << "FAILED_TO_GET_LOCATION_OF_UNIFORM: " << name << std::endl;
	}

	glUniform1i(uniformLocation, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	int uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
	if (uniformLocation == -1) {
		std::cout << "FAILED_TO_GET_LOCATION_OF_UNIFORM: " << name << std::endl;
	}

	glUniform1f(uniformLocation, value);
}

#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	unsigned int vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

	// compile shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// error checking
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR_SHADER_PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
	unsigned int vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
	unsigned int geometryShader = compileShader(geometryPath, GL_GEOMETRY_SHADER);

	// compile shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);

	// error checking
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR_SHADER_PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

void Shader::use() const
{
	glUseProgram(shaderProgram);
}

void Shader::setActiveTextures()
{
	for (const std::pair<std::string, std::string>& c : configuration) 
	{
		const std::string& uniform_name = c.first;
		const std::string& texture_name = c.second;
		glActiveTexture(GL_TEXTURE0 + uniformMap.at(uniform_name));
		glBindTexture(GL_TEXTURE_2D, textureMap.at(texture_name));
	}
}

void Shader::addTexture(const std::string& path, const std::string& textureName)
{
	unsigned int textureID = loadTexture(path.c_str());
	textureMap[textureName] = textureID;
}

void Shader::addTextures(const std::vector<std::string>& paths, const std::vector<std::string>& textureNames)
{
	int n = paths.size();
	std::vector<std::thread> threads;
	threads.reserve(n);
	unsigned int* textureIDs = new unsigned int[n];
	std::vector<int> width(n), height(n), nrComponents(n);
	std::vector<unsigned char*> data(n);
	std::vector<GLenum> format(n);
	glGenTextures(n, textureIDs);

	for (int i = 0; i < n; i++) {
		threads.emplace_back(&Shader::loadTextureThread, this, paths[i].c_str(), textureIDs[i], &width[i], &height[i], &nrComponents[i], &data[i], std::ref(format[i]));
	}
	for (int i = 0; i < n; i++) {
		threads[i].join();
	}

	for (int i = 0; i < n; i++) {
		// bind the texture
		glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
		// set the texture format
		glTexImage2D(GL_TEXTURE_2D, 0, format[i], width[i], height[i], 0, format[i], GL_UNSIGNED_BYTE, data[i]);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// store the texture in the texture map
		textureMap[textureNames[i]] = textureIDs[i];
		stbi_image_free(data[i]);
	}

	delete[] textureIDs;
}

void Shader::addTextureUniform(const std::string& uniform_name, int location)
{
	uniformMap[uniform_name] = location;
	configuration[uniform_name] = "not set";
	use();
	setInt(uniform_name, location);
}

void Shader::changeMaterial(const std::string& uniform_name, const std::string& texture_name)
{
	configuration.at(uniform_name) = texture_name;
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

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		// set the texture format
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping/filtering options (on the currently bound texture object)
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

void Shader::loadTextureThread(char const* path, unsigned int textureID, int* width, int* height, int* nrComponents, unsigned char** data, GLenum& format)
{
	*data = stbi_load(path, width, height, nrComponents, 0);
	if (data)
	{
		if (*nrComponents == 1)
			format = GL_RED;
		else if (*nrComponents == 3)
			format = GL_RGB;
		else if (*nrComponents == 4)
			format = GL_RGBA;

	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
}

unsigned int Shader::compileShader(const std::string& path, GLenum type)
{
	unsigned int shader;

	// retrieve the shader code from the shader file
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open file
		shaderFile.open(path);
		std::stringstream shaderStream;
		// read file's buffer contents into stream
		shaderStream << shaderFile.rdbuf();
		// close file handler
		shaderFile.close();
		// convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* shaderSource = shaderCode.c_str();

	// link the shader file with the program and compile	
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	// error checking
	int success;
	char infoLog[512];
	std::string typeString;
	switch (type)
	{
		case GL_VERTEX_SHADER:
			typeString = "VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			typeString = "FRAGMENT";
			break;
		case GL_GEOMETRY_SHADER:
			typeString = "GEOMETRY";
			break;
	}
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR_" << typeString << "_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
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

int Shader::getTextureCount() const
{
	return textureMap.size();
}

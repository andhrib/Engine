#include "WoodenTable.h"

WoodenTable::WoodenTable(std::vector<glm::vec3>& lightCubePositions, const glm::vec3& cameraPos, glm::vec3& dirLightDirection) : 
model("res/models/wooden_table/Wooden Table.dae"),
shader("res/shaders/vertex/woodenTable.vert", "res/shaders/fragment/woodenTable.frag"),
rotationMat(1.0f), axis(0.0f, 1.0f, 0.0f), rotSpeed(15.0f), position(0.0f), 
dirLightDirection(dirLightDirection), 
lightingType(POINT_LIGHT)
{
	// set the textures
    shader.use();
	shader.addTexture("res/models/wooden_table/Albedo.jpg", "wood_albedo");
	shader.addTexture("res/models/wooden_table/Metallic.jpg", "wood_specular");
	shader.addTexture("res/textures/metal/metal_albedo.jpg", "metal_albedo");
	shader.addTexture("res/textures/metal/metal_specular.jpg", "metal_specular");
	shader.addTexture("res/textures/rock/rock_albedo.jpg", "rock_albedo");
	shader.addTexture("res/textures/rock/rock_specular.jpg", "rock_specular");
	shader.addTexture("res/models/wooden_table/Normal.png", "normal");

	// set the texture uniforms
	shader.addTextureUniform("u_material.texture_albedo", 0);
	shader.addTextureUniform("u_material.texture_specular", 1);
	shader.addTextureUniform("u_material.texture_normal", 2);

	// the default material is wood
	shader.changeMaterial("u_material.texture_albedo", "wood_albedo");
	shader.changeMaterial("u_material.texture_specular", "wood_specular");
	shader.changeMaterial("u_material.texture_normal", "normal");
	setMaterial(WOOD);

	// set the camera position
	shader.setVec3("u_viewPos", cameraPos);

	// set the point light as the default lighting type
	shader.setInt("u_lightingType", lightingType);

	// set the point light positions
	setLightPositions(lightCubePositions);

	// set the directional light direction in the shader
	shader.setVec3("u_dirLightDirection", dirLightDirection);

	// set the shadow far plane
	shader.setFloat("u_farPlane", POINT_SHADOW_FAR);

	// make the table 10 times smaller than the original size
    rotationMat = glm::scale(rotationMat, glm::vec3(0.1f));
}

void WoodenTable::draw(glm::mat4& view, glm::mat4& projection)
{
    // set the uniforms
    shader.use();
    shader.setMat4("u_view", view);
    shader.setMat4("u_projection", projection);
	shader.setMat4("u_model", modelMat);

    model.draw(shader);
}

void WoodenTable::setLightingType(LightingType lt)
{
	if (lt != lightingType)
	{
		lightingType = lt;
		shader.use();
		shader.setInt("u_lightingType", lightingType);
	}
}

void WoodenTable::setMaterial(MaterialType ct)
{
	shader.use();
	switch (ct) {
	case WOOD:
		shader.changeMaterial("u_material.texture_albedo", "wood_albedo");
		shader.changeMaterial("u_material.texture_specular", "wood_specular");
		shader.setFloat("u_material.ambientStrength", 0.005f);
		shader.setFloat("u_material.diffuseStrength", 1.0f);
		shader.setFloat("u_material.specularStrength", 0.5f);
		shader.setFloat("u_material.shininess", 32.0f);
		break;
	case METAL:
		shader.changeMaterial("u_material.texture_albedo", "metal_albedo");
		shader.changeMaterial("u_material.texture_specular", "metal_specular");
		shader.setFloat("u_material.ambientStrength", 0.005f);
		shader.setFloat("u_material.diffuseStrength", 1.0f);
		shader.setFloat("u_material.specularStrength", 0.5f);
		shader.setFloat("u_material.shininess", 32.0f);
		break;
	case ROCK:
		shader.changeMaterial("u_material.texture_albedo", "rock_albedo");
		shader.changeMaterial("u_material.texture_specular", "rock_specular");
		shader.setFloat("u_material.ambientStrength", 0.005f);
		shader.setFloat("u_material.diffuseStrength", 1.0f);
		shader.setFloat("u_material.specularStrength", 0.5f);
		shader.setFloat("u_material.shininess", 32.0f);
		break;

	}
}

void WoodenTable::drawShadow(Shader& shadowShader)
{
	shadowShader.use();
	shadowShader.setMat4("u_model", modelMat);
	model.draw(shadowShader);
}

void WoodenTable::updateModelMatrix(float deltaTime)
{
	// model rotation
	if (glm::length(axis) != 0) { // if axis == (0, 0, 0), remain still
		rotationMat = glm::rotate(rotationMat, glm::radians(deltaTime) * rotSpeed, axis);
	}
	// model translation
	translationMat = glm::translate(glm::mat4(1.0f), position);
	modelMat = translationMat * rotationMat;
}

void WoodenTable::setPointShadowMaps(std::vector<unsigned int>& depthCubemaps)
{
	int startIdx = shader.getTextureCount();
	shader.use();
	for (int i = 0; i < depthCubemaps.size(); i++)
	{
		shader.setInt("u_pointShadowMap[" + std::to_string(i) + "]", startIdx + i);
		glActiveTexture(GL_TEXTURE0 + startIdx + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemaps[i]);
	}
}

void WoodenTable::setDirShadowMap(unsigned int depthMap)
{
	shader.use();
	shader.setInt("u_dirShadowMap", shader.getTextureCount());
	glActiveTexture(GL_TEXTURE0 + shader.getTextureCount());
	glBindTexture(GL_TEXTURE_2D, depthMap);
}

void WoodenTable::setLightSpaceMatrix(glm::mat4 lightSpaceMatrix)
{
	shader.use();
	shader.setMat4("u_lightSpaceMatrix", lightSpaceMatrix);
}

void WoodenTable::setLightPositions(std::vector<glm::vec3>& lightCubePositions)
{
	for (int i = 0; i < lightCubePositions.size(); i++)
    {
		shader.use();
		shader.setVec3("u_lightPositions[" + std::to_string(i) + "]", lightCubePositions[i]);
	}
}

#include "WoodenTable.h"

WoodenTable::WoodenTable(std::vector<glm::vec3>& lightCubePositions, glm::vec3 cameraPos) : model("res/models/wooden_table/Wooden Table.dae"),
shader("res/shaders/vertex/woodenTable.vert", "res/shaders/fragment/woodenTable.frag"),
rotationMat(1.0f), axis(0.0f, 1.0f, 0.0f), rotSpeed(15.0f), position(0.0f), dirLightDirection(-0.2f, -1.0f, -0.3f), 
lightingType(POINT_LIGHT)
{
	// set the textures
    shader.use();
	shader.addTexture("res/models/wooden_table/Albedo.jpg", "wood_albedo");
	shader.addTexture("res/models/wooden_table/Metallic.jpg", "wood_specular");
	shader.addTexture("res/textures/metal/metal_albedo.jpg", "metal_albedo");
	shader.addTexture("res/textures/metal/metal_specular.jpg", "metal_specular");

	// set the texture uniforms
	shader.addTextureUniform("u_material.texture_albedo", 0);
	shader.addTextureUniform("u_material.texture_specular", 1);

	// the default configuration is wood
	shader.changeConfiguration("u_material.texture_albedo", "wood_albedo");
	shader.changeConfiguration("u_material.texture_specular", "wood_specular");

	// set the camera position
	shader.setVec3("u_cameraPos", cameraPos);

	// set the point light as the default lighting type
	shader.setInt("u_lightingType", lightingType);

	// set the point light positions
	setLightPositions(lightCubePositions);

	// set the directional light direction in the shader
	shader.setVec3("u_dirLightDirection", dirLightDirection);

	// make the table 10 times smaller than the original size
    rotationMat = glm::scale(rotationMat, glm::vec3(0.1f));
}

void WoodenTable::draw(glm::mat4& view, glm::mat4& projection, float deltaTime)
{
    // set the uniforms
    shader.use();
    shader.setMat4("u_view", view);
    shader.setMat4("u_projection", projection);
    // model rotation
    if (glm::length(axis) != 0) { // if axis == (0, 0, 0), remain still
        rotationMat = glm::rotate(rotationMat, glm::radians(deltaTime) * rotSpeed, axis);
    }
	// model translation
	glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
    shader.setMat4("u_model", translationMat * rotationMat);

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

void WoodenTable::setConfiguration(ConfigurationType ct)
{
	if (ct == WOOD)
	{
		shader.changeConfiguration("u_material.texture_albedo", "wood_albedo");
		shader.changeConfiguration("u_material.texture_specular", "wood_specular");
	}
	else if (ct == METAL)
	{
		shader.changeConfiguration("u_material.texture_albedo", "metal_albedo");
		shader.changeConfiguration("u_material.texture_specular", "metal_specular");
	}
}

void WoodenTable::setLightPositions(std::vector<glm::vec3>& lightCubePositions)
{
	for (int i = 0; i < lightCubePositions.size(); i++)
    {
		shader.use();
		shader.setVec3("u_lightPositions[" + std::to_string(i) + "]", lightCubePositions[i]);
	}
}

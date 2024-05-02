#include "WoodenTable.h"

WoodenTable::WoodenTable() : model("res/models/wooden_table/Wooden Table.dae"),
shader("res/shaders/vertex/woodenTable.vert", "res/shaders/fragment/woodenTable.frag"),
modelMat(1.0f), axis(0.0f, 1.0f, 0.0f), rotSpeed(15.0f)
{
    shader.use();
	shader.addTexture("res/models/wooden_table/Albedo.jpg");
    shader.setInt("u_material.texture_albedo", 0);

    modelMat = glm::scale(modelMat, glm::vec3(0.1f));
}

void WoodenTable::draw(glm::mat4& view, glm::mat4& projection, float deltaTime)
{
    // set the uniforms
    shader.use();
    shader.setMat4("u_view", view);
    shader.setMat4("u_projection", projection);
    // model rotation
    if (glm::length(axis) != 0) { // if axis == (0, 0, 0), remain still
        modelMat = glm::rotate(modelMat, glm::radians(deltaTime) * rotSpeed, axis);
        shader.setMat4("u_model", modelMat);
    }
    model.draw(shader);
}

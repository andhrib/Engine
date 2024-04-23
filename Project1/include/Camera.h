#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Camera
{
protected: 
	glm::vec3 position; // the (x, y, z) position of the camera in world space

	glm::vec3 front; // the direction the camera is facing
	glm::vec3 right; // the right direction of the camera; it is perpendicular to the front direction
	glm::vec3 up; // the up direction of the camera; it is perpendicular to both the front and right directions
	glm::vec3 worldUp; // the world up direction, typically equal to (0, 1, 0)

	float pitch; // the angle between the (x, z)-plane and the view direction, interval (-90°, 90°)
	float yaw; // the angle between the x-axis and the view direction, interval [0°, 360°)

	float fov; // the FOV
	float sensitivity; // the sensitivity of the mouse input

public:
	Camera();
	void cursorInput(GLFWwindow* window, float xOffset, float yOffset); // process the input from moving the cursor
	void scrollInput(GLFWwindow* window, float value); // process the input from using the scroll wheel
	virtual void keyInput(GLFWwindow* window, float deltaTime) = 0; // process the input from pressing keys
	glm::mat4 getViewMatrix() const; // generate and return the corresponding view matrix
	glm::vec3 getPosition() const; // return the position vector
	glm::vec3 getFront() const; // return the front vector
	float getFOV() const;
};



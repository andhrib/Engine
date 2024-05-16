#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// default values
const glm::vec3 POSITION = glm::vec3(0.0f);
const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 WORLDUP = glm::vec3(0.0f, 1.0f, 0.0f);
const float PITCH = 0.0f;
const float YAW = 270.0f;
const float FOV = 45.0f;
const float SENSITIVITY = 0.1f;

class Camera
{
private: 
	// the (x, y, z) position of the camera in world space
	glm::vec3 position;

	// the direction the camera is facing
	glm::vec3 front;
	// the right direction of the camera; it is perpendicular to the front direction
	glm::vec3 right;
	// the up direction of the camera; it is perpendicular to both the front and right directions
	glm::vec3 up;
	// the world up direction, typically equal to (0, 1, 0)
	glm::vec3 worldUp;

	// the angle between the (x, z)-plane and the view direction, interval (-90°, 90°)
	float pitch;
	// the angle between the x-axis and the view direction, interval [0°, 360°)
	float yaw; 

	// the field of view
	float fov;
	// the sensitivity of the mouse input
	float sensitivity; 

public:
	Camera();
	Camera(const glm::vec3& position);
	// process the input from using the scroll wheel
	void scrollInput(GLFWwindow* window, float value);
	// process the input from moving the mouse
	void mouseInput(GLFWwindow* window, double xoffset, double yoffset);
	// getters
	glm::mat4& getViewMatrix() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getFront() const;
	float getFOV() const;
};



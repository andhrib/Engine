#include "Camera.h"

// sets the default camera values
Camera::Camera() : position(vec3(0.0f)), front(vec3(0.0f, 0.0f, -1.0f)), 
up(vec3(0.0f, 1.0f, 0.0f)), right(vec3(1.0f, 0.0f, 0.0f)), worldUp(vec3(0.0f, 1.0f, 0.0f)), 
pitch(0.0f), yaw(270.0f), fov(45.0f), sensitivity(0.1f)
{
}

void Camera::cursorInput(GLFWwindow* window, float xOffset, float yOffset)
{
	// update the angle values
	yaw += xOffset * sensitivity;
	pitch += yOffset * sensitivity;

	// keep the pitch in the range (-90°, 90°)
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	// update the front vector
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = normalize(front);

	// update the right vector
	right = normalize(glm::cross(front, worldUp));

	// update the up vector
	up = normalize(glm::cross(right, front));
}

void Camera::scrollInput(GLFWwindow* window, float value)
{
	fov += value;

	// keep the FOV in the range (1.0f, 45.0f)
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	else if (fov > 45.0f) {
		fov = 45.0f;
	}
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

glm::vec3 Camera::getFront() const
{
	return front;
}

float Camera::getFOV() const
{
	return fov;
}

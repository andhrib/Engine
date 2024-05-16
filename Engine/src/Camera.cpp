#include "Camera.h"

// sets the default camera values
Camera::Camera() : Camera(POSITION)
{
}

Camera::Camera(const glm::vec3& position) : position(position), front(FRONT), up(UP), right(RIGHT), 
worldUp(WORLDUP), pitch(PITCH), yaw(YAW), fov(FOV), sensitivity(SENSITIVITY) 
{
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

void Camera::mouseInput(GLFWwindow* window, double xoffset, double yoffset)
{
	// update the yaw and pitch values while taking into account the sensitivity
	yaw += xoffset * sensitivity;
	pitch += yoffset * sensitivity;

	// keep the pitch in the range (-89.0f, 89.0f) to avoid flipping the camera
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	// update the front, right, and up vectors using the updated yaw and pitch values
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));	
}

glm::mat4& Camera::getViewMatrix() const
{
	glm::mat4 view = glm::lookAt(position, position + front, up);
	return view;
}

const glm::vec3& Camera::getPosition() const
{
	return position;
}

const glm::vec3& Camera::getFront() const
{
	return front;
}

float Camera::getFOV() const
{
	return fov;
}

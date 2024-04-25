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

#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Forward, Up);
}

void Camera::ProcessKeyboard(CAMERA_MOVEMENT direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Forward * velocity;
	if (direction == BACKWARD)
		Position -= Forward * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	// Make sure that when pitch is out of bounds, the scren doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Forward, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
	Zoom -= (float)yOffset;

	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	// Calculate the new Forward Vector
	glm::vec3 forward;
	forward.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	forward.y = sin(glm::radians(Pitch));
	forward.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Forward = glm::normalize(forward);

	// also re-calculate the Right and Up vectors
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

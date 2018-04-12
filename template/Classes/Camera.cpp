#include "Camera.h"

//=======================================================================
//                         Constructor/Destructor
//=======================================================================

Camera::Camera(glm::vec3 _position, glm::vec3 _up)
{
	front = glm::vec3(0.0f, -0.0f, -1.0f); // the direction it is looking at
	rotation = glm::vec3(0);
	lookingVector = glm::vec3(0);
	movementSpeed = 10.0f;
	zoom = 45;

	position = _position;
	up = _up;

	updateCameraVectors();
}
Camera::~Camera()
{

}

//=======================================================================
//                         Public Methods
//=======================================================================
glm::mat4 Camera::update()
{
	glm::mat4 result = glm::lookAt(position, lookingVector, up);
	result = glm::rotate(result, rotation.x, glm::vec3(1, 0, 0));
	result = glm::rotate(result, rotation.y, glm::vec3(0, 1, 0));
	result = glm::rotate(result, rotation.z, glm::vec3(0, 0, 1));
	return result;
}

//=======================================================================
//                         Event Listeners
//=======================================================================
void Camera::keyboardEventListener(int direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == 0)
		position += front * velocity;
	if (direction == 1)
		position -= front * velocity;
	if (direction == 2)
		position -= right * velocity;
	if (direction == 3)
		position += right * velocity;
	if (direction == 4)
		position += up * velocity;
	if (direction == 5)
		position -= up * velocity;

	if (direction == 6)
		front.x -= velocity;

	if (direction == 7)
		front.x += velocity;
}

void Camera::mouseEventListener(float xoffset, float yoffset, bool constrainPitch = true)
{

}


//=======================================================================
//                         Private Methods
//=======================================================================
// Disclaimer: the explanation for the math can be found at https://learnopengl.com/Getting-started/Camera
void Camera::updateCameraVectors()
{
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));
}


//=======================================================================
//                         Testing purposes TODO: Delete
//=======================================================================
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
}
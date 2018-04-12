#ifndef Camera_H
#define Camera_H

#pragma once

#include <vector>
#include <string>
#include <glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	Camera(glm::vec3 position, glm::vec3 up);
	~Camera();

	//=======================================================================
	//							  Public Methods
	//=======================================================================
	glm::mat4 update();

	//=======================================================================
	//							  Event Listeners
	//=======================================================================
	void keyboardEventListener(int direction, float deltaTime);
	void mouseEventListener(float xoffset, float yoffset, bool constrainPitch);

	//=======================================================================
	//							  Properties
	//=======================================================================
	glm::vec3 position;
	glm::vec3 front; // not currently used as we are always focussed on origin
	glm::vec3 up;
	glm::vec3 right;

	float movementSpeed;
	float zoom;

	glm::vec3 rotation; 
	glm::vec3 lookingVector;
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

protected:

	//=======================================================================
	//							  Private Methods
	//=======================================================================
	void updateCameraVectors();

};

#endif

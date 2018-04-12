#ifndef Horse_H
#define Horse_H

#pragma once

#include <vector>
#include <string>
#include <glm.hpp>
#include "Cube.h"
#include "Complex.h"

class Horse : public Complex
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	Horse(glm::vec3 _position);
	~Horse();
	void keyboardEventListener(GLFWwindow* window, int direction, EventFlags eventFlags, float deltaTime, std::vector<Complex*> subscribedObjects);
	void AI();
	void varySpeed();
	int generateSteps();
	int generateWait();

	int generateDir();

	//=======================================================================
	//							AI variables
	//=======================================================================
	int steps = 0;
	int wait = 0;
	int rotating = 0;
	int speedChange = 0;
	//=======================================================================
	//							Event Listeners
	//=======================================================================

};

#endif

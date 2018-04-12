#ifndef Complex_H
#define Complex_H

#pragma once

#include <vector>
#include <string>
#include <../glm/gtc/matrix_transform.hpp>
#include "Object.h"
#include "Part.h"
#include <glfw3.h>

struct EventFlags
{
	float capsLock = false;
	float allowTextures = true;
	float allowAi = true;
};

class Complex
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	Complex();
	virtual ~Complex();
	virtual void keyboardEventListener(GLFWwindow* window, int direction, EventFlags eventFlags, float deltaTime, std::vector<Complex*> subscribedObjects);

	virtual void AI();
	void teleport(int x, int z);
	glm::vec3 calculateDirection();
	void move(float distance);
	bool ai = true;
	int aiSleep = 0;

	float speed = .6f;
	//=======================================================================
	//                         Properties
	//=======================================================================
	Part base;
	std::string name ="";
};

#endif


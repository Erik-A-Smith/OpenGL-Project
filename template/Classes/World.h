#ifndef World_H
#define World_H

#pragma once

#include <vector>
#include <string>
#include <glfw3.h>
#include <glm.hpp>
#include "Complex.h"
#include "Cube.h"

class World : public Complex
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	World(int length = 1, int width = 1);
	~World();

	//=======================================================================
	//							 Properties
	//=======================================================================
	float length;
	float width;
};

#endif

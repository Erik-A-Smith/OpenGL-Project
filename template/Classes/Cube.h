#ifndef Cube_H
#define Cube_H

#pragma once

#include <vector>
#include <string>
#include "Object.h"

class Cube : public Object
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	Cube(glm::vec3 _position, glm::vec3 _up);
	~Cube();

	//=======================================================================
	//							  Properties
	//=======================================================================
	std::vector<glm::vec3> vector;
};

#endif

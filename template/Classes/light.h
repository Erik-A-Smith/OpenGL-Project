#ifndef Light_H
#define Light_H

#pragma once

#include <vector>
#include <string>
#include <glm.hpp>

class Light
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	Light(glm::vec3 _position, glm::vec3 _up);
	~Light();

	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 lightDir;
	glm::vec3 color;
	float strength  = 0.1f;
	
	//=======================================================================
	//							  Properties
	//=======================================================================

};

#endif

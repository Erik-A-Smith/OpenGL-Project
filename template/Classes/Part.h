#ifndef Part_H
#define Part_H

#pragma once

#include <vector>
#include <string>
#include "Object.h"

class Part
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	Part(Object partRepresentation);
	Part();
	~Part();

	//=======================================================================
	//							  Properties
	//=======================================================================
	Object partRepresentation;
	std::vector<Part*> subParts;
};

#endif

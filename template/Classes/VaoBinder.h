#pragma once

#ifndef VaoBinder_H
#define VaoBinder_H

#pragma once

#include <glm.hpp>
#include <vector>
#include <string>
#include "Part.h"
#include <glew.h>


class VaoBinder
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	VaoBinder();
	~VaoBinder();

	//=======================================================================
	//                         Methods
	//=======================================================================
	void bind(Part* object);
};

#endif
#include "Light.h"

//=======================================================================
//                         Constructor/Destructor
//=======================================================================
Light::Light(glm::vec3 _position, glm::vec3 _up)
{
	position = _position;
	up = _up;
	color = glm::vec3(1);
	lightDir = _up;
}

Light::~Light()
{

}


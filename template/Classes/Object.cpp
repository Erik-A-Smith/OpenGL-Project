#include "Object.h"



//=======================================================================
//                         Constructor/Destructor
//=======================================================================
Object::Object()
{
	// Defaults
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	Object(position, up);
}

Object::Object(glm::vec3 _position, glm::vec3 _up)
{
	position = _position;
	up = _up;
	front = glm::vec3(1.0f, 0.0f, 0.0f);

	// Defaults
	scale = glm::vec3(1, 1, 1);
	angles = glm::vec3(0, 0, 0);
	color = Color(20, 20, 20, 1);

	//updateVectors();
}

Object::~Object()
{

}

// Copy Constructor
Object::Object(Object* &d2)
{ 
	// Necessary properties
	vertices = d2->vertices;
	indices = d2->indices;
	normals = d2->normals;
	uv = d2->uv;

	// Spacials
	orientation = d2->orientation;
	scale = d2->scale;
	angles = d2->angles;
	position = d2->position;

	// Directionals
	front = d2->front;
	up = d2->up;
	right = d2->right;

	// Visuals
	color = d2->color;

}

glm::mat4 Object::update(glm::mat4 parent)
{

	glm::mat4 result = parent;
	
	// Update local rotation
	result = glm::rotate(result, angles.x, glm::vec3(1, 0, 0));
	result = glm::rotate(result, angles.y, glm::vec3(0, 1, 0));
	result = glm::rotate(result, angles.z, glm::vec3(0, 0, 1));
	
	// Update position
	result = glm::translate(result, position);
	
	// Orientation
	result = glm::rotate(result, orientation.x, glm::vec3(1, 0, 0));
	result = glm::rotate(result, orientation.y, glm::vec3(0, 1, 0));
	result = glm::rotate(result, orientation.z, glm::vec3(0, 0, 1));
	
	 //Mouvements
	result = glm::translate(result, rotation);
	
	// Spacial rotation
	result = glm::rotate(result, rotation.x, glm::vec3(1, 0, 0));
	result = glm::rotate(result, rotation.y, glm::vec3(0, 1, 0));
	result = glm::rotate(result, rotation.z, glm::vec3(0, 0, 1));
	
	// Scale
	result = glm::scale(result, scale);

	return result;
}
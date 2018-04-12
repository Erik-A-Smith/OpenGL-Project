#ifndef Drawable_H
#define Drawable_H

#pragma once

#include <vector>
#include <string>
#include <glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>

struct Color
{
	float r,g,b,a;

	Color() {
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}

	Color(float _r, float _g, float _b, float _a) {
		r = _r / 255;
		g = _g / 255;
		b = _b / 255;
		a = _a;
	}

	Color(glm::vec4 _colors) {
		r = _colors.x / 255;
		g = _colors.y / 255;
		b = _colors.z / 255;
		a = _colors.y;
	}

	Color& operator=(glm::vec4 _colors) {
		r = _colors.x / 255;
		g = _colors.y / 255;
		b = _colors.z / 255;
		a = _colors.y / 255;
		return *this;
	}

	glm::vec4 Color::vec() {
		return (glm::vec4(r,g,b,a));
	}

};

class Object
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	Object(); // Constructor with default spacial definitions
	Object(glm::vec3 _position, glm::vec3 _up); // Constructor with spacial definitions
	Object(Object* &d2); // Copy Constructor
	virtual ~Object();

	//=======================================================================
	//							  Public Methods
	//=======================================================================
	glm::mat4 update(glm::mat4 parent);
	//void updateVectors();

	//=======================================================================
	//                         Properties
	//=======================================================================

	//Saves
	unsigned vaoIndex = NULL;

	// Necessary properties
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> normals = {};
	std::vector<float> uv = {};
	std::vector<float> texCoords = {};

	// Spacials
	glm::vec3 orientation; // relative orientation
	glm::vec3 scale;  // object scale vector
	glm::vec3 angles; //x,y,z
	glm::vec3 position; // 3d coordinates
	glm::vec3 rotation; // new! its the _rotation from Mouvements

	// Directionals
	glm::vec3 front; // where the object is facing
	glm::vec3 up; // where the object is facing
	glm::vec3 right; // where the object is facing

	// Mouvement
	float movementSpeed = 2.5f;

	// Visuals
	Color color;
	std::string texture = "";
};

#endif


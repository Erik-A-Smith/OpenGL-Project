#include "Axis.h"

//=======================================================================
//                         Constructor/Destructor
//=======================================================================
Axis::Axis() : Complex()
{
	// Base cube
	base = Part(Object(glm::vec3(0.0f, -1.75f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	//Generate Cardinat indicators
	Cube cube = Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//cardinalX
	cube.scale = glm::vec3(1, .2, .2);
	cube.rotation = glm::vec3(0, 0, 0);
	cube.color = glm::vec4(255, 0, 0, 1);

	cube.position = glm::vec3(0.5, 1, 0);
	Part* x1 = new Part(cube);
	base.subParts.push_back(x1);

	cube.position = glm::vec3(1.5, 1, 0);
	Part* x2 = new Part(cube);
	base.subParts.push_back(x2);

	cube.position = glm::vec3(2.5, 1, 0);
	Part* x3 = new Part(cube);
	base.subParts.push_back(x3);

	cube.position = glm::vec3(3.5, 1, 0);
	Part* x4 = new Part(cube);
	base.subParts.push_back(x4);

	cube.position = glm::vec3(4.5, 1, 0);
	Part* x5 = new Part(cube);
	base.subParts.push_back(x5);

	//cardinalY
	cube.scale = glm::vec3(.2, 1., .2);
	cube.rotation = glm::vec3(0, 0, 0);
	cube.color = glm::vec4(0, 255, 0, 1);

	cube.position = glm::vec3(0, 1.5, 0);
	Part* y1 = new Part(cube);
	base.subParts.push_back(y1);

	cube.position = glm::vec3(0, 2.5, 0);
	Part* y2 = new Part(cube);
	base.subParts.push_back(y2);

	cube.position = glm::vec3(0, 3.5, 0);
	Part* y3 = new Part(cube);
	base.subParts.push_back(y3);

	cube.position = glm::vec3(0, 4.5, 0);
	Part* y4 = new Part(cube);
	base.subParts.push_back(y4);

	cube.position = glm::vec3(0, 5.5, 0);
	Part* y5 = new Part(cube);
	base.subParts.push_back(y5);

	//cardinalX
	cube.scale = glm::vec3(.2, .2, 1);
	cube.rotation = glm::vec3(0, 0, 0);
	cube.color = glm::vec4(0, 0, 255, 1);

	cube.position = glm::vec3(0, 1, 0.5);
	Part* z1 = new Part(cube);
	base.subParts.push_back(z1);

	cube.position = glm::vec3(0, 1, 1.5);
	Part* z2 = new Part(cube);
	base.subParts.push_back(z2);

	cube.position = glm::vec3(0, 1, 2.5);
	Part* z3 = new Part(cube);
	base.subParts.push_back(z3);

	cube.position = glm::vec3(0, 1, 3.5);
	Part* z4 = new Part(cube);
	base.subParts.push_back(z4);

	cube.position = glm::vec3(0, 1, 4.5);
	Part* z5 = new Part(cube);
	base.subParts.push_back(z5);
}

Axis::~Axis()
{

}


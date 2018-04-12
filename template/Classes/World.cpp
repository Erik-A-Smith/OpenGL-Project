#include "World.h"


//=======================================================================
//                         Constructor/Destructor
//=======================================================================
World::World(int _length, int _width) : Complex()
{
	length = _length;
	width = _width;

	base = Part(Object(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	Cube cube = Cube(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	cube.scale = glm::vec3(_length, .05, _width);
	cube.color = glm::vec4(10, 200, 10, 1);
	cube.texture = "grass";
	base.subParts.push_back(new Part(cube));
}

World::~World()
{

}
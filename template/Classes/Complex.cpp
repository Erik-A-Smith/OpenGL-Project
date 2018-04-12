#include "Complex.h"

//=======================================================================
//                         Constructor/Destructor
//=======================================================================
Complex::Complex()
{
	
}
Complex::~Complex()
{

}

void Complex::keyboardEventListener(GLFWwindow * window, int direction, EventFlags eventFlags, float deltaTime, std::vector<Complex*> subscribedObjects)
{
}

void Complex::AI() {
}

void Complex::teleport(int x, int z) {
	base.partRepresentation.position.x += x;
	base.partRepresentation.position.z += z;
}

// Calculates objects front
glm::vec3 Complex::calculateDirection() {
	glm::vec3 newVec = base.partRepresentation.front;
	float angle = base.partRepresentation.orientation.y;

	glm::mat4 rotationMat(1); // Creates a identity matrix
	rotationMat = glm::rotate(rotationMat, angle, glm::vec3(0.0, 1.0, 0.0));
	newVec = glm::vec3(rotationMat * glm::vec4(newVec, 1.0));
	return newVec;
}

void Complex::move(float distance)
{
	glm::vec3 dir = calculateDirection();
	base.partRepresentation.position.x += distance * dir.x;
	//base.partRepresentation.position.y += distance * dir.y;
	base.partRepresentation.position.z += distance * dir.z;
}

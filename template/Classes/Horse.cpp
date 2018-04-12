#include "Horse.h"

Horse::Horse(glm::vec3 _position) : Complex()
{
	// Name object
	name = "Horse";

	// --
	glm::vec4 colorBody = glm::vec4(97,65,38,1);
	glm::vec4 colorLower = glm::vec4(77,52,30, 1);
	glm::vec4 colorUpper = glm::vec4(112,84,59, 1);
	glm::vec4 colorNeck = glm::vec4(56,42,29, 1);
	glm::vec4 colorHead = glm::vec4(77, 52, 30, 1);
	glm::vec4 colorJoint = glm::vec4(255, 255, 255, 1);
	
	// Base cube
	base = Part(Object(_position, glm::vec3(0.0f, 1.0f, 0.0f)) );

	//--
	Cube cube = Cube(base.partRepresentation.position, glm::vec3(0.0f, 1.0f, 0.0f));
	cube.texture = "horse";

	cube.scale = glm::vec3(2, 1, 2);
	cube.position = glm::vec3(0, 1, 0);
	cube.color = colorBody;
	cube.rotation = glm::vec3(0, 0, 0);
	cube.orientation = glm::vec3(0, 0, 0);
	Part* torso = new Part(cube);
	base.subParts.push_back(torso);

	//--
	cube.scale = glm::vec3(1.5, .5, .5);
	cube.position = glm::vec3(1, torso->partRepresentation.position.y + .7, -.75);
	cube.color = colorNeck;
	cube.rotation = glm::vec3(0, 0, glm::radians(45.0f));
	Part* neck = new Part(cube);
	base.subParts.push_back(neck);

	//---
	cube.scale = glm::vec3(1.5, .5, .5);
	cube.position = glm::vec3(neck->partRepresentation.position.x + 1.03, neck->partRepresentation.position.y + .33, neck->partRepresentation.position.z +1.57);
	cube.color = colorHead;
	cube.rotation = glm::vec3(0, 0, glm::radians(-45.0f));
	Part* head = new Part(cube);
	base.subParts.push_back(head);

	//---
	cube.scale = glm::vec3(.25, .75, .25);
	cube.position = glm::vec3(.80, torso->partRepresentation.position.y - .83, .75);
	cube.color = colorUpper;
	cube.rotation = glm::vec3(0, 0, 0);
	Part* front_upper_right_leg = new Part(cube);
	base.subParts.push_back(front_upper_right_leg);
	
	//---
	cube.scale = glm::vec3(.25, .75, .25);
	cube.position = glm::vec3(front_upper_right_leg->partRepresentation.position.x, front_upper_right_leg->partRepresentation.position.y - .75, front_upper_right_leg->partRepresentation.position.z);
	cube.color = colorLower;
	cube.rotation = glm::vec3(0, 0, 0);
	Part* front_lower_right_leg = new Part(cube);
	base.subParts.push_back(front_lower_right_leg);

	//---
	cube.scale = glm::vec3(.25, .75, .25);
	cube.position = glm::vec3(-.80, torso->partRepresentation.position.y - .83, .75);
	cube.color = colorUpper;
	cube.rotation = glm::vec3(0, 0, 0);
	Part* hind_upper_right_leg = new Part(cube);
	base.subParts.push_back(hind_upper_right_leg);

	//---
	cube.scale = glm::vec3(.25, .75, .25);
	cube.position = glm::vec3(hind_upper_right_leg->partRepresentation.position.x, hind_upper_right_leg->partRepresentation.position.y - .75, hind_upper_right_leg->partRepresentation.position.z);
	cube.color = colorLower;
	cube.rotation = glm::vec3(0, 0, 0);
	Part* hind_lower_right_leg = new Part(cube);
	base.subParts.push_back(hind_lower_right_leg);

	//---
	cube.scale = glm::vec3(.25, .75, .25);
	cube.position = glm::vec3(.80, torso->partRepresentation.position.y - .83, -.75);
	cube.color = colorUpper;
	cube.rotation = glm::vec3(0, 0, 0);
	Part* front_upper_left_leg = new Part(cube);
	base.subParts.push_back(front_upper_left_leg);

	//---
	cube.scale = glm::vec3(.25, .75, .25);
	cube.position = glm::vec3(front_upper_left_leg->partRepresentation.position.x, front_upper_left_leg->partRepresentation.position.y - .75, front_upper_left_leg->partRepresentation.position.z);
	cube.color = colorLower;
	cube.rotation = glm::vec3(0, 0, 0);
	Part* front_lower_left_leg = new Part(cube);
	base.subParts.push_back(front_lower_left_leg);

	//---
	cube.scale = glm::vec3(.25, .75, .25);
	cube.position = glm::vec3(-.80, torso->partRepresentation.position.y - .83, -.75);
	cube.color = colorUpper;
	cube.rotation = glm::vec3(0, 0, 0);
	Part* hind_upper_left_leg = new Part(cube);
	base.subParts.push_back(hind_upper_left_leg);

	//---
	cube.scale = glm::vec3(.25, .75, .25);
	cube.position = glm::vec3(hind_upper_left_leg->partRepresentation.position.x, hind_upper_left_leg->partRepresentation.position.y - .75, hind_upper_left_leg->partRepresentation.position.z);
	cube.color = colorLower;
	cube.rotation = glm::vec3(0, 0, 0);
	Part* hind_lower_left_leg = new Part(cube);
	base.subParts.push_back(hind_lower_left_leg);
}

Horse::~Horse()
{
}

void Horse::keyboardEventListener(GLFWwindow* window, int key, EventFlags eventFlags, float deltaTime, std::vector<Complex*> subscribedObjects)
{
	// Displacement
	if (!eventFlags.capsLock) {

		// Settings
		//float MoveDistance = .6f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			move(speed);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			move(-speed);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			float rotation = glm::radians(-10.0f);
			base.partRepresentation.orientation.y = base.partRepresentation.orientation.y + rotation;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			float rotation = glm::radians(10.0f);
			base.partRepresentation.orientation.y = base.partRepresentation.orientation.y + rotation;
		}

		// Diagonal mouvement, horses do not mouve diagonally.
		// More like car drives then a crab walks
		//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		//	base.partRepresentation.position.x -= 1.0f;
		//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//	base.partRepresentation.position.z -= 1.0f;//velocity;
		//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		//	base.partRepresentation.position.z += 1.0f;//velocity;
	}
	else {
		// Rotate
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			float rotation = glm::radians(10.0f);
			base.partRepresentation.orientation.y = base.partRepresentation.orientation.y + rotation;
		}

		// Rotate Y
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			float rotation = glm::radians(-10.0f);
			base.partRepresentation.orientation.y = base.partRepresentation.orientation.y + rotation;
		}

		// Rotate Z
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			float rotation = glm::radians(10.0f);
			base.partRepresentation.orientation.z = base.partRepresentation.orientation.z + rotation;
		}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			float rotation = glm::radians(10.0f);
			base.partRepresentation.orientation.z = base.partRepresentation.orientation.z - rotation;
		}
	}

	// Scale
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
		float scaling = 1.5 / 1.2;
		base.partRepresentation.scale *= 1.2 / 1;
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
		float scaling = 1.2 / 1.5;
		base.partRepresentation.scale *= scaling;
	}

}


void Horse::AI() {
	if (ai){

		if (aiSleep != 0){
			aiSleep--;
			return;
		}

		// Settings
		//float MoveDistance = .1f;
		float rotationPerTic = glm::radians(1.0f);

		// Speed varyance 
		varySpeed();

		// Move
		if (wait != 0) {
			wait--;

			if (wait == 0) {
				steps = generateSteps();
			}

			return;
		}

		// Move
		if (steps != 0) {
			move(speed);
			steps--;

			if (steps == 0) {
				rotating = generateDir();
			}

			return;
		}

		// Rotate
		if (rotating != 0) {
			base.partRepresentation.orientation.y = base.partRepresentation.orientation.y + (rotationPerTic * (rotating / abs(rotating)));

			if (rotating < 0) {
				rotating++;
			}
			else {
				rotating--;
			}

			if (rotating == 0) {
				wait = generateWait();
			}

			return;
		}

		steps = generateSteps();
	}
	
}
void Horse::varySpeed() {
	if (speedChange != 0) {
		speedChange--;
		return;
	}
	else {
		speed = (float) (rand() % 10) / 25.0f;
		speedChange = (rand() % 20);
	}
}
int Horse::generateSteps() {
	return (rand() % 60);
}

int Horse::generateWait() {
	return (rand() % 30);
}

int Horse::generateDir() {
	int totalAngle = 30;
	return (rand() % totalAngle) - (totalAngle/2);
}

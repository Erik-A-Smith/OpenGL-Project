//////////////////////////////////
//		Generic Includes
//////////////////////////////////
#include "stdafx.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "stb_image.h"
#include <glm.hpp>
#include <glew.h>
#include <glfw3.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

//////////////////////////////////
//		Class Includes
//////////////////////////////////
#include "Classes/Shader.h"
#include "Classes/VaoBinder.h"
#include "Classes/TextureBinder.h"
#include "Classes/Cube.h"
#include "Classes/Horse.h"
#include "Classes/World.h"
#include "Classes/Camera.h"
#include "Classes/Axis.h"
#include "Classes/Light.h"
#include "Classes/CmplCube.h"

//////////////////////////////////
//		Function Signatures
//////////////////////////////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void clearScreen();
bool init(GLFWwindow* &window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void window_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
bool renderfps(int framerate);

//////////////////////////////////
//		Object declarations
//////////////////////////////////
std::vector<Complex*> subscribedObjects; // List of object that need to be drawn
//std::vector<Light*> subscribedLighs; // List of object that need to be drawn
VaoBinder vaoBinder;
TextureBinder textureBinder;
float currentTimeRender = glfwGetTime();
float lastTimeRender = currentTimeRender;
float lastMouseX = NULL;
float lastMouseY = NULL;
float capsLock = false;
float allowTextures = true;

float SCREEN_WIDTH = 800;
float SCREEN_HEIGHT = 800;

int WORLD_LENGTH = 50;
int WORLD_WIDTH = 50;

glm::vec3 cameraStartingPosition = glm::vec3(0.0f, 6.0f, 20.0f);
glm::vec3 cameraStartingAngle = glm::vec3(0.0f, 0.0f, 0.0f);
Horse* horse = new Horse(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
CmplCube* cmplCube = new CmplCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
World* world = new World(WORLD_LENGTH, WORLD_WIDTH);
Light* light = new Light(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
Axis* axis = new Axis();

// camera
Camera* camera = new Camera(cameraStartingPosition, glm::vec3(0.0f, 1.0f, 0.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

// App specific variables ( used in smoothing camera mouvement)
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float currentFrame = 0.0f; // Curent time of the app. Baseline for calculation.

// Event listener flags
bool firstMouse = true; // is this the first time capturing the mouse mouvement?

//////////////////////////////////
//			Main
//////////////////////////////////
int main() {

	//////////////////////////////////
	//		Constant declarations
	//////////////////////////////////
	const std::string VERTEX_SHADER_PATH = "Shaders/vertex.shader";
	const std::string FRAGMENT_SHADER_PATH = "Shaders/fragment.shader";
	const std::string VERTEX_SHADER_LIGHT_PATH = "Shaders/vertex_light.shader";
	const std::string FRAGMENT_SHADER_LIGHT_PATH = "Shaders/fragment_light.shader";

	//////////////////////////////////
	//		Initialisation
	//////////////////////////////////
	GLFWwindow* window;
	if (!init(window)) {
		std::cout << "There was a problem initialising!";
		return -1;
	}
	Shader* shader = new Shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	Shader* shader_light = new Shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	//////////////////////////////////
	//		Variable declarations
	//////////////////////////////////
	subscribedObjects.push_back(world);
	//subscribedObjects.push_back(horse);
	subscribedObjects.push_back(axis);
	subscribedObjects.push_back(cmplCube);
	//subscribedLighs.push_back(light);

	//////////////////////////////////
	//	Compile VAO's for all objects
	//////////////////////////////////
	std::vector<unsigned int> VBOs, EBOs;
	for (std::vector<Complex*>::iterator cmplx = subscribedObjects.begin(); cmplx != subscribedObjects.end(); ++cmplx) {
		Object* tmp = (*cmplx)->geometricObject;
		vaoBinder.bind(*cmplx, VBOs, EBOs);
	}

	//////////////////////////////////
	//		Settings
	//////////////////////////////////
	glEnable(GL_DEPTH_TEST); // Corrects the z buffer


	//////////////////////////////////
	//			Testing
	//////////////////////////////////
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, 1);// maybe?!?
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//////////////////////////////////
	//		Render lights
	//////////////////////////////////
	shader->setVec3("lightPosition", light->position);

	//////////////////////////////////
	//		Main Loop
	//////////////////////////////////
	while (!glfwWindowShouldClose(window))
	{
		shader->setVec3("lightPosition", light->position);

		//////////////////////////////////
		//		Time Calculations
		//////////////////////////////////
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (renderfps(24)) {
			//////////////////////////////////
			//			Camera
			//////////////////////////////////

			// camera/view transformation
			glm::mat4 projection = glm::perspective(camera->zoom, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
			shader->setMat4("projection", projection);

			shader->setMat4("view", camera->update());

			// Clear screen & z buffer
			clearScreen();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



			//////////////////////////////////
			//		Render lights
			//////////////////////////////////
			// be sure to activate shader when setting uniforms/drawing objects
			shader_light->activate();
			shader_light->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			shader_light->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
			shader_light->setVec3("lightPos", lightPos);
			shader_light->setVec3("viewPos", camera->position);

			// view/projection transformations
			glm::mat4 projection2 = glm::perspective(glm::radians(camera->zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera->update();
			shader_light->setMat4("projection", projection2);
			shader_light->setMat4("view", view);

			//////////////////////////////////
			//		Rendering
			//////////////////////////////////
			for (std::vector<VAOObjects>::iterator vao = vaoBinder.bound.begin(); vao != vaoBinder.bound.end(); ++vao) {
				shader->activate();
				for (std::vector<Mouvement*>::iterator mvmnt = vao->complex->buildStack.begin(); mvmnt != vao->complex->buildStack.end(); ++mvmnt) {

					// calculate scale/rotation/position
					glm::mat4 objectModel = (vao)->complex->geometricObject->update(*mvmnt);

					shader->setMat4("model", objectModel);

					//bind the vao to be drawn
					glBindVertexArray((vao)->vao);
					//glBindVertexArray(lightVAO);

					// bind textures on corresponding texture units
					if ( (vao)->complex->geometricObject->texture != "" && allowTextures) {
						shader->setBool("hasTexture", true);
						glBindTexture(GL_TEXTURE_2D, textureBinder.textureByName((vao)->complex->geometricObject->texture));
					}
					else {
						shader->setBool("hasTexture", false);
						glBindTexture(GL_TEXTURE_2D, 0);
					}
					// Color
					shader->setVec4("color", (vao)->complex->geometricObject->color->r, (vao)->complex->geometricObject->color->g, (vao)->complex->geometricObject->color->b, (vao)->complex->geometricObject->color->a);

					// Drawing element
					glDrawElements(GL_TRIANGLES, sizeof((vao)->complex->geometricObject->getIndices()) * 3, GL_UNSIGNED_INT, 0); // draw the object!

				}

			}
		}

		//////////////////////////////////
		//	Check events and swap buffers
		//////////////////////////////////
		glfwSwapBuffers(window); // double buffered, we swap it here
		glfwPollEvents(); // check for event listeners
	}

	//////////////////////////////////
	//	De-allocate all resources
	//////////////////////////////////
	/*for (std::vector<unsigned int>::iterator it = VBOs.begin(); it != VBOs.end(); ++it) {
		glDeleteBuffers(1, &(*it));
	}*/
	for (std::vector<unsigned int>::iterator it = EBOs.begin(); it != EBOs.end(); ++it) {
		glDeleteBuffers(1, &(*it));
	}

	// Clean memory pointers
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void clearScreen() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

bool init(GLFWwindow* &window)
{
	//////////////////////////////////
	//		Initialize GLFW
	//////////////////////////////////
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//////////////////////////////////
	//		Make Context
	//////////////////////////////////
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	//////////////////////////////////
	//		Innitialize glew
	//////////////////////////////////
	if (glewInit() != 0) {
		return false;
	}

	//////////////////////////////////
	//		Set Event Listeners
	//////////////////////////////////
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//////////////////////////////////
	//		Init textures
	//////////////////////////////////
	textureBinder.init();
	return true;
}

//////////////////////////////////
//		Event Listeners
//////////////////////////////////
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Camera mouvement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera->keyboardEventListener(0, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera->keyboardEventListener(1, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera->keyboardEventListener(2, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera->keyboardEventListener(3, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		camera->keyboardEventListener(6, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		camera->keyboardEventListener(7, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
		camera->position = cameraStartingPosition;
		camera->rotation = glm::vec3(0);
	}

	// Horse Displacement
	if (!capsLock) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			horse->keyboardEventListener(0, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			horse->keyboardEventListener(1, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			horse->keyboardEventListener(2, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			horse->keyboardEventListener(3, deltaTime);
	}
	else {

		// Y rotate
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			horse->keyboardEventListener(4, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			horse->keyboardEventListener(5, deltaTime);

		// z Rotate
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			horse->keyboardEventListener(8, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			horse->keyboardEventListener(9, deltaTime);

	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		horse->keyboardEventListener(6, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		horse->keyboardEventListener(7, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		int newX = (rand() % WORLD_LENGTH) - WORLD_LENGTH / 2;
		int newZ = (rand() % WORLD_WIDTH) - WORLD_WIDTH / 2;
		horse->geometricObject->position.x = newX;
		horse->geometricObject->position.z = newZ;
	}

	// Escape
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// Polygon modes
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Boolean Flags
	if (key == GLFW_KEY_CAPS_LOCK && action == GLFW_PRESS) {
		capsLock = !capsLock;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		allowTextures = !allowTextures;
	}

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		camera->position.x += (lastMouseX - xpos) * .05;
		camera->position.y -= (lastMouseY - ypos) * .05;
		lastMouseX = xpos;
		lastMouseY = ypos;

	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		camera->position.z -= (lastMouseY - ypos) * .05;
		lastMouseX = xpos;
		lastMouseY = ypos;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
		camera->rotation.y += glm::radians((lastMouseX - xpos));
		camera->rotation.x += glm::radians((lastMouseY - ypos));
		lastMouseX = xpos;
		lastMouseY = ypos;
	}else {
		lastMouseX = xpos;
		lastMouseY = ypos;
	}
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{

}
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	float zoomNew = camera->zoom;
	zoomNew -= yoffset * .05;
	camera->zoom = zoomNew;
}

bool renderfps(int framerate)
{
	float currentTimeRender = glfwGetTime();

	if (currentTimeRender - lastTimeRender >= 1.0 / framerate)
	{
		lastTimeRender = currentTimeRender;
		return true;
	}
}

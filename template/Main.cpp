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
#include "Classes/Horse.h"
#include "Classes/World.h"
#include "Classes/Camera.h"
#include "Classes/Axis.h"
#include "Classes/Light.h"

//////////////////////////////////
//		Function Signatures
//////////////////////////////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void clearScreen();
bool init(GLFWwindow* &window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void window_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
bool renderfps(int framerate);
void renderPart(Shader shader, Part* part, glm::mat4 parent);
bool outOfBounds(Complex* object, World* world);
float sqr(float number);
bool collision(Complex* colider, Complex* colidee);
Complex* searchByName(std::vector<Complex*> vector, std::string name);


// List of object that need to be drawn
std::vector<Complex*> subscribedObjects;
std::vector<Light*> subscribedLights;
std::vector<Complex*> eventableObjects;
std::vector<Complex*> collidableObjects;

//Object binders
VaoBinder vaoBinder;
TextureBinder textureBinder;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Settings
int WORLD_LENGTH = 100;
int WORLD_WIDTH = 100;
int NUMBER_OF_HORSES = 20;
int FPS = 60;

//horse
Horse* horse = new Horse(glm::vec3(0, 0, 0));

//world
World* world = new World(WORLD_LENGTH, WORLD_WIDTH);

Axis* axis = new Axis();

// camera
glm::vec3 cameraStartingPosition = glm::vec3(0.0f, 6.0f, 20.0f);
glm::vec3 cameraStartingAngle = glm::vec3(0.0f, 0.0f, 0.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0,1,0));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float lastMouseX = NULL;
float lastMouseY = NULL;

// timing
float currentTimeRender = glfwGetTime();
float lastTimeRender = currentTimeRender;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Event Flags
EventFlags eventFlags;
int main()
{
	//////////////////////////////////
	//		Object Declarations
	//////////////////////////////////
	Light* light = new Light(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0, 1.0f, 0));
	light->lightDir = glm::vec3(glm::vec3(0.0f, 1.0f, 0.0f));


;	//////////////////////////////////
	//		Constant declarations
	//////////////////////////////////
	const std::string VERTEX_SHADER_PATH = "Shaders/vertex.shader";
	const std::string FRAGMENT_SHADER_PATH = "Shaders/fragment.shader";

	//////////////////////////////////
	//		Initialisation
	//////////////////////////////////
	GLFWwindow* window;
	if (!init(window)) {
		std::cout << "There was a problem initialising!";
		return -1;
	}

	//////////////////////////////////
	//		Build Shaders
	//////////////////////////////////
	Shader shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	//////////////////////////////////
	//	Subscribe Object to be drawn
	//////////////////////////////////
	subscribedObjects.push_back(world);
	subscribedObjects.push_back(axis);

	// Generate the herd
	for (size_t i = 0; i < NUMBER_OF_HORSES; i++)
	{
		// Calculate random spacial position
		int newX = (rand() % WORLD_LENGTH) - WORLD_LENGTH / 2;
		int newZ = (rand() % WORLD_WIDTH) - WORLD_WIDTH / 2;

		// Calculate random orientation
		float newOrientation = glm::radians((float) (rand() % 360) );

		//Spawn horse
		Horse* horseCp = new Horse(glm::vec3(newX, 0, newZ));
		horseCp->base.partRepresentation.orientation.y = newOrientation;

		// Spawn protection from spawning a horse inside another horse
		bool spawnProtection = true;
		while (spawnProtection)
		{
			for (std::vector<Complex*>::iterator collidee = collidableObjects.begin(); collidee != collidableObjects.end(); ++collidee) {
				if (collision(horseCp, *collidee)) {
					newX = (rand() % WORLD_LENGTH) - WORLD_LENGTH / 2;
					newZ = (rand() % WORLD_WIDTH) - WORLD_WIDTH / 2;
					horseCp->teleport(newX, newZ);
					continue;
				}
			}
			break;
		}


		// Draw the horses
		subscribedObjects.push_back(horseCp);

		// Horses are collidable
		collidableObjects.push_back(horseCp);

		// Controle all the horses
		//eventableObjects.push_back(horseCp);
		//horseCp->ai = false;
	}

	//////////////////////////////////
	//Testing horse
	//////////////////////////////////
	subscribedObjects.push_back(horse);
	collidableObjects.push_back(horse);
	eventableObjects.push_back(horse);
	horse->ai = false;

	//////////////////////////////////
	// Subscribe lights to be rendered
	//////////////////////////////////
	subscribedLights.push_back(light);

	//////////////////////////////////
	//		Bind Vao cube
	//////////////////////////////////
	for (std::vector<Complex*>::iterator cmplx = subscribedObjects.begin(); cmplx != subscribedObjects.end(); ++cmplx) {
		vaoBinder.bind(&(*cmplx)->base);
	}

	//////////////////////////////////
	//		Render Loop
	//////////////////////////////////
	while (!glfwWindowShouldClose(window))
	{
		if (renderfps(FPS)) {
			//////////////////////////////////
			//	render frame time counter
			//////////////////////////////////
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			//////////////////////////////////
			//		Reset render bits
			//////////////////////////////////
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//////////////////////////////////
			//	  Projection calculations
			//////////////////////////////////
			glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.update();
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);

			//////////////////////////////////
			//		Render Lights
			//////////////////////////////////
			for (std::vector<Light*>::iterator lght = subscribedLights.begin(); lght != subscribedLights.end(); ++lght) {
				shader.activate();
				shader.setFloat("ambientStrength", (*lght)->strength);
				shader.setVec3("lightColor", (*lght)->color);//1.0f, 1.0f, 1.0f);
				shader.setVec3("lightPos", (*lght)->position);
				shader.setVec3("lightDir", (*lght)->lightDir);
				shader.setVec3("viewPos", camera.position);
			}

			//////////////////////////////////
			//		Render Objects
			//////////////////////////////////
			for (std::vector<Complex*>::iterator cplx = subscribedObjects.begin(); cplx != subscribedObjects.end(); ++cplx) {
				Part* tmp = &(*cplx)->base;
				renderPart(shader, tmp, glm::mat4());

				// Execute AI
				if (eventFlags.allowAi) {
					(*cplx)->AI();
				}

				//out of bounds
				if (eventFlags.allowAi && outOfBounds(*cplx, world)) {

					float turnPerTic = 10.0f;
					int turns = 360 / turnPerTic;
					while (true)
					{
						(*cplx)->base.partRepresentation.orientation.y += glm::radians(turnPerTic);
						(*cplx)->move(1);

						if (!outOfBounds(*cplx, world)) {
							(*cplx)->move(-1);
							break;
						}
						(*cplx)->move(-1);

						turns--;
						if (turns == 0) {
							(*cplx)->aiSleep = 5;
							break;
						}
					}

				}
			}
			//////////////////////////////////
			//		Collision Detection
			//////////////////////////////////
			for (std::vector<Complex*>::iterator collidee = collidableObjects.begin(); collidee != collidableObjects.end(); ++collidee) {

				for (std::vector<Complex*>::iterator collider = collidableObjects.begin(); collider != collidableObjects.end(); ++collider) {

					if ((*collidee)->ai && eventFlags.allowAi) {
						if (collidee != collider && collision(*collidee, *collider)) {
							if ((*collidee)->aiSleep == 0) {

								// Stops infinite loop if object cannot stop colliding
								float turnPerTic = 10.0f;
								int turns = 360 / turnPerTic;
								while (true)
								{
									(*collidee)->base.partRepresentation.orientation.y += glm::radians(turnPerTic);
									(*collidee)->move(1);

									if (!collision(*collidee, *collider)) {
										(*collidee)->move(-1);
										break;
									}
									(*collidee)->move(-1);

									turns--;

									if (turns == 0) {
										(*collidee)->aiSleep = 5;
										break;
									}
								}
								(*collider)->aiSleep = 5;
							}
						}
					}
				}
			}

			//////////////////////////////////
			//		Buffer swapping
			//////////////////////////////////
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	//////////////////////////////////
	//			Free memory
	//////////////////////////////////
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &cube->vaoIndex);
	//glDeleteVertexArrays(1, &lightVAO);
	//glDeleteBuffers(1, &VBO);

	//////////////////////////////////
	//				End
	//////////////////////////////////
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

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	float zoomNew = camera.zoom;
	zoomNew -= yoffset;
	camera.zoom = zoomNew;
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
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
	glfwSetScrollCallback(window, scroll_callback);

	//////////////////////////////////
	//		Init textures
	//////////////////////////////////
	textureBinder.init();

	//////////////////////////////////
	//		GL Settings (z-buffer)
	//////////////////////////////////
	glEnable(GL_DEPTH_TEST);
	return true;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//////////////////////////////////
//		Event Listeners
//////////////////////////////////
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		camera.position.x += (lastMouseX - xpos) * .05;
		camera.position.y -= (lastMouseY - ypos) * .05;
		lastMouseX = xpos;
		lastMouseY = ypos;

	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		camera.position.z -= (lastMouseY - ypos) * .05;
		lastMouseX = xpos;
		lastMouseY = ypos;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
		camera.rotation.y += glm::radians((lastMouseX - xpos));
		camera.rotation.x += glm::radians((lastMouseY - ypos));
		lastMouseX = xpos;
		lastMouseY = ypos;
	}
	else {
		lastMouseX = xpos;
		lastMouseY = ypos;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Camera mouvement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.keyboardEventListener(0, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.keyboardEventListener(1, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.keyboardEventListener(2, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.keyboardEventListener(3, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		camera.keyboardEventListener(6, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		camera.keyboardEventListener(7, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
		camera.position = cameraStartingPosition;
		camera.rotation = glm::vec3(0);
	}
	
	for (std::vector<Complex*>::iterator cplx = eventableObjects.begin(); cplx != eventableObjects.end(); ++cplx) {
		(*cplx)->keyboardEventListener(window, key, eventFlags, deltaTime, subscribedObjects);
	}

	// Teleport horse
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		for (std::vector<Complex*>::iterator cplx = eventableObjects.begin(); cplx != eventableObjects.end(); ++cplx) {
			int newX = (rand() % WORLD_LENGTH) - WORLD_LENGTH / 2;
			int newZ = (rand() % WORLD_WIDTH) - WORLD_WIDTH / 2;
			(*cplx)->teleport(newX, newZ);
		}
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
		eventFlags.capsLock = !eventFlags.capsLock;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		eventFlags.allowTextures = !eventFlags.allowTextures;
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		eventFlags.allowAi = !eventFlags.allowAi;
	}
}

void renderPart(Shader shader, Part* part, glm::mat4 parent) {

	// Make the model (Apply its transformations)
	if ((part->partRepresentation).vaoIndex != NULL) {

		// bind textures on corresponding texture units
		if (part->partRepresentation.texture != "" && eventFlags.allowTextures) {
			shader.setBool("enabledTextures", true);
			glBindTexture(GL_TEXTURE_2D, textureBinder.textureByName(part->partRepresentation.texture));
		}
		else {
			shader.setBool("enabledTextures", false);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		shader.setMat4("model", part->partRepresentation.update(parent));

		// Render Objects color
		shader.setVec3("objectColor", part->partRepresentation.color.vec());
		glBindVertexArray((part->partRepresentation).vaoIndex);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glm::mat4 newParent = glm::mat4();
	newParent = part->partRepresentation.update(parent);
	for (std::vector<Part*>::iterator prt = part->subParts.begin(); prt != part->subParts.end(); ++prt) {
		renderPart(shader, (*prt), newParent);
	}

}

Complex* searchByName(std::vector<Complex*> vector, std::string name) {
	for (std::vector<Complex*>::iterator cplx = vector.begin(); cplx != vector.end(); ++cplx) {
		if ((*cplx)->name == name) {
			return (*cplx);
		}
	}
}

bool renderfps(int framerate)
{
	float currentTimeRender = glfwGetTime();

	if (currentTimeRender - lastTimeRender >= 1.0 / framerate)
	{
		lastTimeRender = currentTimeRender;
		return true;
	}
	return false;
}


bool outOfBounds(Complex* object, World* world) {
	if (
		object->base.partRepresentation.position.x > (world->width /2) ||
		object->base.partRepresentation.position.x < -(world->width / 2) ||
		object->base.partRepresentation.position.z < -(world->length / 2) ||
		object->base.partRepresentation.position.z > (world->length / 2)
		) 
	{
		return true;
	}

	object->base.partRepresentation.color = glm::vec4(0, 0, 0, 1);
	return false;
}

bool collision(Complex* colider, Complex* colidee) {
	// Colliding Box Radius
	float CBR = 2;
	float xDiff = colidee->base.partRepresentation.position.x - colider->base.partRepresentation.position.x;
	float zDiff = colidee->base.partRepresentation.position.z - colider->base.partRepresentation.position.z;
	float tmpx = sqr(xDiff);
	float tmpz = sqr(zDiff);
	float tmpDiff = tmpx + tmpz;
	float distance = sqrt(tmpDiff);

	if (distance < (CBR * 2) ) {
		colider->base.partRepresentation.color = glm::vec4(255, 0, 0, 1);
		return true;
	}
	return false;
}

float sqr(float number) {
	return number * number;
}
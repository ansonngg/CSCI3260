/*
Student Information
Student ID: 1155110182
Student Name: NG Ka Wai
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Draw.h"

Shader shader;
Shader skyboxShader;

// Camera
glm::vec3 eye(0.0f, 10.0f, -15.0f);
glm::vec3 center(0.0f, 0.0f, 15.0f);

// Spacecraft
glm::vec3 spacecraftPos(0.0f);
float spacecraftAngle = glm::radians(90.0f);
float spacecraftSpeed = 50.0f;

// Vehicle
float vehicleAngle = 0.0f;
float vehicleRotate = 1.0f;

// Planet
float planetAngle = 0.0f;
float planetRotate = 0.5f;

// Rock
GLuint rockAmount = 200;
glm::mat4 rockModelMatrices[200];
float rockAngle = 0.0f;
float rockRotate = 0.1f;
float rockSelfAngle = 0.0f;
float rockSelfRotate = 1.0f;

// Time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct KeyboardController {
	bool UP = false;
	bool DOWN = false;
	bool LEFT = false;
	bool RIGHT = false;
} keyboardCtl;

struct MouseController {
	double oldx = 0.0, oldy = 0.0;
	float sensitivity = 0.002f;
} mouseCtl;

// screen setting
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

Texture universeSkybox;
Model spacecraftObj = loadOBJ("objects/spacecraft.obj");
Texture spacecraftTexture[2];
Model alienObj = loadOBJ("objects/alienpeople.obj");
Texture alienTexture;
Model vehicleObj = loadOBJ("objects/alienvehicle.obj");
Texture vehicleTexture[2];
Model planetObj = loadOBJ("objects/planet.obj");
Texture planetTexture[2];
Model rockObj = loadOBJ("objects/rock.obj");
Texture rockTexture;
Model chickenObj = loadOBJ("objects/chicken.obj");
Texture chickenTexture;

GLuint vao[7];
GLuint vbo[7];
GLuint ebo[7];
GLuint* buffers[] = { vao, vbo, ebo };

void sendDataToOpenGL()
{
	// Generate buffers
	glGenVertexArrays(7, vao);
	glGenBuffers(7, vbo);
	glGenBuffers(7, ebo);

	// 0 Universe Skybox
	const char* universeFaces[] = {
		"textures/universe skybox/right.bmp",
		"textures/universe skybox/left.bmp",
		"textures/universe skybox/top.bmp",
		"textures/universe skybox/bottom.bmp",
		"textures/universe skybox/front.bmp",
		"textures/universe skybox/back.bmp"
	};
	universeSkybox.setupCubemap(universeFaces);
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vao[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	// 1 Spacecraft
	spacecraftTexture[0].setupTexture("textures/spacecraftTexture.bmp");
	spacecraftTexture[1].setupTexture("textures/leisure_spacecraftTexture.bmp");
	Draw(buffers, 1, spacecraftObj);

	// 2 Alien
	alienTexture.setupTexture("textures/alienTexture.bmp");
	Draw(buffers, 2, alienObj);

	// 3 Vehicle
	vehicleTexture[0].setupTexture("textures/alienTexture.bmp");
	vehicleTexture[1].setupTexture("textures/colorful_alien_vehicleTexture.bmp");
	Draw(buffers, 3, vehicleObj);

	// 4 Planet
	planetTexture[0].setupTexture("textures/planetTexture.bmp");
	planetTexture[1].setupTexture("textures/planetNormal.bmp");
	Draw(buffers, 4, planetObj);

	// 5 Rock
	rockTexture.setupTexture("textures/rockTexture.bmp");
	Draw(buffers, 5, rockObj);

	// 6 Chicken
	chickenTexture.setupTexture("textures/chickenTexture.bmp");
	Draw(buffers, 6, chickenObj);
}

void initializedGL(void) //run only once
{
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	shader.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	skyboxShader.setupShader("SkyboxVertexShader.glsl", "SkyboxFragmentShader.glsl");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	CreateRockModel(rockModelMatrices, rockAmount);
}

void paintGL(void)  //always run
{
	/* Skybox Part */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	skyboxShader.use();

	// viewMatrix
	glm::mat4 viewMatrix = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 skyboxViewMatrix = glm::mat4(glm::mat3(viewMatrix));
	skyboxShader.setMat4("viewMatrix", skyboxViewMatrix);
	// projectionMatrix
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);
	skyboxShader.setMat4("projectionMatrix", projectionMatrix);

	// 0 Universe Skybox
	glBindVertexArray(vao[0]);
	universeSkybox.bindCubemap(0);
	skyboxShader.setInt("skybox", 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	/* Drawing Part */
	glDepthMask(GL_TRUE);
	shader.use();

	// eyeWorldMatrix
	glm::mat4 eyeWorldMatrix = projectionMatrix * viewMatrix;
	shader.setMat4("eyeWorldMatrix", eyeWorldMatrix);

	// Time
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Spacecraft movement
	float spacecraftSpeedTemp = spacecraftSpeed * deltaTime;
	// Difference w.r.t. front
	glm::vec2 fdiffTemp(glm::cos(spacecraftAngle), glm::sin(spacecraftAngle));
	glm::vec2 fdiff(0.0f);
	// Difference w.r.t. left
	glm::vec2 ldiffTemp(glm::cos(spacecraftAngle - glm::radians(90.0f)), glm::sin(spacecraftAngle - glm::radians(90.0f)));
	glm::vec2 ldiff(0.0f);
	if (keyboardCtl.UP)
		fdiff += fdiffTemp;
	if (keyboardCtl.DOWN)
		fdiff -= fdiffTemp;
	if (keyboardCtl.LEFT)
		ldiff += ldiffTemp;
	if (keyboardCtl.RIGHT)
		ldiff -= ldiffTemp;
	// Totall difference
	glm::vec2 fldiff = fdiff + ldiff;
	// Normalize if moving diagonally
	if ((keyboardCtl.UP || keyboardCtl.DOWN) && (keyboardCtl.LEFT || keyboardCtl.RIGHT))
		fldiff = glm::normalize(fldiff);
	fldiff *= spacecraftSpeedTemp;
	spacecraftPos.x += fldiff.x;
	spacecraftPos.z += fldiff.y;
	eye.x += fldiff.x;
	eye.z += fldiff.y;
	center.x += fldiff.x;
	center.z += fldiff.y;

	// Vehicle rotation
	vehicleAngle += vehicleRotate * deltaTime;
	if (vehicleAngle > glm::radians(360.0f))
		vehicleAngle -= glm::radians(360.0f);

	// Planet rotation
	planetAngle += planetRotate * deltaTime;
	if (planetAngle > glm::radians(360.0f))
		planetAngle -= glm::radians(360.0f);

	// Rock rotation
	rockAngle += rockRotate * deltaTime;
	if (rockAngle > glm::radians(360.0f))
		rockAngle -= glm::radians(360.0f);
	rockSelfAngle += rockSelfRotate * deltaTime;
	if (rockSelfAngle > glm::radians(360.0f))
		rockSelfAngle -= glm::radians(360.0f);

	// 1 Spacecraft
	glBindVertexArray(vao[1]);
	spacecraftTexture[0].bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), spacecraftPos);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f) - spacecraftAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, spacecraftObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 2 Alien 1
	glBindVertexArray(vao[2]);
	alienTexture.bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 5.14f, 100.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, alienObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 2 Alien 2
	glBindVertexArray(vao[2]);
	alienTexture.bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 5.14f, 200.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, alienObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 2 Alien 3
	glBindVertexArray(vao[2]);
	alienTexture.bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 5.14f, 300.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, alienObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 3 Vehicle 1
	glDisable(GL_CULL_FACE);
	glBindVertexArray(vao[3]);
	vehicleTexture[0].bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.0f, 100.0f));
	modelMatrix = glm::rotate(modelMatrix, vehicleAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, vehicleObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 3 Vehicle 2
	glBindVertexArray(vao[3]);
	vehicleTexture[0].bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 0.0f, 200.0f));
	modelMatrix = glm::rotate(modelMatrix, vehicleAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, vehicleObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 3 Vehicle 3
	glBindVertexArray(vao[3]);
	vehicleTexture[0].bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.0f, 300.0f));
	modelMatrix = glm::rotate(modelMatrix, vehicleAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, vehicleObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);

	// 4 Planet
	glBindVertexArray(vao[4]);
	planetTexture[0].bind(0);
	shader.setInt("material.myTexture", 0);
	planetTexture[1].bind(1);
	shader.setInt("material.myNormalTexture", 1);
	shader.setInt("normalMappingFlag", 1);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -15.0f, 500.0f));
	modelMatrix = glm::rotate(modelMatrix, planetAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(15.0f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, planetObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 5 Rock
	glBindVertexArray(vao[5]);
	rockTexture.bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	glm::mat4 modelMatrixTemp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 500.0f));
	modelMatrixTemp = glm::rotate(modelMatrixTemp, rockAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 selfRotateMatrix = glm::rotate(glm::mat4(1.0f), rockSelfAngle, glm::vec3(0.4f, 0.6f, 0.8f));
	for (GLuint i = 0; i < rockAmount; i++) {
		modelMatrix = modelMatrixTemp * rockModelMatrices[i] * selfRotateMatrix;
		shader.setMat4("modelMatrix", modelMatrix);
		glDrawElements(GL_TRIANGLES, rockObj.indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	// 6 Chicken 1
	glBindVertexArray(vao[6]);
	chickenTexture.bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 3.0f, 100.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, chickenObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 6 Chicken 2
	glBindVertexArray(vao[6]);
	chickenTexture.bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 3.0f, 200.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, chickenObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// 6 Chicken 3
	glBindVertexArray(vao[6]);
	chickenTexture.bind(0);
	shader.setInt("material.myTexture", 0);
	shader.setInt("normalMappingFlag", 0);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 3.0f, 300.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, chickenObj.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Sets the mouse-button callback for the current window.	
}

bool firstMouse = true;

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	// Sets the cursor position callback for the current window
	if (firstMouse) {
		mouseCtl.oldx = x;
		mouseCtl.oldy = y;
		firstMouse = false;
	}
	float rotationSpeed = mouseCtl.sensitivity * (mouseCtl.oldx - x);
	spacecraftAngle -= rotationSpeed;
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	// Align the axis of rotation with the y-axis
	eye -= spacecraftPos;
	center -= spacecraftPos;
	// Rotate
	eye = glm::vec3(rotationMatrix * glm::vec4(eye, 1.0f));
	center = glm::vec3(rotationMatrix * glm::vec4(center, 1.0f));
	// Move back
	eye += spacecraftPos;
	center += spacecraftPos;
	mouseCtl.oldx = x;
	mouseCtl.oldy = y;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Sets the scoll callback for the current window.
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Sets the Keyboard callback for the current window.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		keyboardCtl.UP = true;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		keyboardCtl.DOWN = true;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		keyboardCtl.LEFT = true;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		keyboardCtl.RIGHT = true;
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		keyboardCtl.UP = false;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		keyboardCtl.DOWN = false;
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		keyboardCtl.LEFT = false;
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		keyboardCtl.RIGHT = false;
}


int main(int argc, char* argv[])
{
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 2", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*register callback functions*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);                                                                  //    
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enable raw mouse motion
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	initializedGL();

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
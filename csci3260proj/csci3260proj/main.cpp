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
glm::vec3 eye(0.0f, 2.0f, 10.0f);
glm::vec3 center(0.0f, 0.0f, 2.0f);

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

GLuint vao[2];
GLuint vbo[2];
GLuint ebo[2];
GLuint* buffers[] = { vao, vbo, ebo };

void sendDataToOpenGL()
{
	// Generate buffers
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);
	glGenBuffers(2, ebo);

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
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
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

	// 1 Spacecraft
	glBindVertexArray(vao[1]);
	spacecraftTexture[0].bind(0);
	shader.setInt("material.myTexture", 0);
	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("modelMatrix", modelMatrix);
	glDrawElements(GL_TRIANGLES, spacecraftObj.indices.size(), GL_UNSIGNED_INT, 0);
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

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	// Sets the cursor position callback for the current window
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Sets the scoll callback for the current window.
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Sets the Keyboard callback for the current window.
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
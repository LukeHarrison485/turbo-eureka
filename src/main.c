#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include "shader.h"
#include "camera.h"
#include <string.h>
#include "block.h"

float vertices[] = {
    // positions          // normals             // texture coords
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, // Top-right
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f, // Top-right
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f, // Top-left
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // Bottom-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f, // Top-right
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f, // Top-right
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f, // Bottom-left
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 1.0f, // Top-left
    // Left face
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // Top-right
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // Bottom-left
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f, // Top-left
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // Bottom-left
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // Top-right
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f, // Bottom-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // Top-left
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f, // Top-right
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // Bottom-right
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f, // Bottom-right
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // Top-left
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // Top-right
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f, // Top-left
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f, // Bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // Top-right
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f, // Bottom-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f, // Top-left
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f, // Top-right
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f, // Bottom-right
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f, // Bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f  // Top-left
};

vec3 pointLightPositions[] = {
	{1.0, 3.0, 0.0},
	{1.3, 4.0, 3.0}
};

Camera cam;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow* window);
void generateBuffers(unsigned int* VBO, unsigned int* VAO);
unsigned int compileShader(unsigned int type, const char* source);
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void setFullscreen(GLFWwindow* window);
void createPointLight(int index, unsigned int shaderProgram);

int isFullscreen = 0;
int windowedWidth = 800; 
int windowedHeight = 600; 
int windowedPosX = 0;
int windowedPosY = 0;

float lastX;
float lastY;

int main(void) {
	cam = createCamera((vec3){0.0f, 0.0f, 0.0f} , 3.0f, 45.0f, 0.1f);
	blocks = (block*)malloc(1000 * 1000 * sizeof(block));
	
	textures[DIRT] = loadTexture("dirt.png");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);
	createPerlinNoise(32, 256, 256, 1);
	lastX = (float)windowedWidth / 2.0f;
	lastY = (float)windowedHeight / 2.0f;
	GLFWwindow* window = glfwCreateWindow((isFullscreen) ? 1920 : windowedWidth, (isFullscreen) ? 1080 : windowedHeight, "YEAH!!!!!!!!! BLOCK GAME, THIS IS THE MOST ORGINIAL IDEA I HAVE EVER HAD!!!!!!!!!!!!!", NULL, NULL);
	if(isFullscreen) {
		setFullscreen(window);
	}
	if (window == NULL)
	{
		fprintf_s(stderr, "Failed to load GLFW window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);  

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf_s(stderr, "Failed to initialize GLAD");
		return -1;
	}
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	generateBuffers(&VBO, &VAO);
	Shader basicShader = createShader("shader/basic.vs", "shader/basic.fs");  

	createChunk(64, 1);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glfwSetKeyCallback(window, keyCallback);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame; 

		processInput(window);
		processCameraInput(window, &cam, deltaTime);
		//Drawing code should go here:
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = glfwGetTime();
		basicShader.setFloat(basicShader.shaderProgram, "time", time);

		glUseProgram(basicShader.shaderProgram);

		mat4 model;
		glm_mat4_identity(model);
		glm_rotate(model, -1.0f * glfwGetTime(), (vec3){1.0f, 0.0f, 0.5f});

		mat4 view;
		glm_mat4_identity(view);
		updateView(cam, view);

		if (isFullscreen) {
			glViewport(0, 0, 1920, 1080); // Update viewport
			// Update projection matrix (if needed)
			mat4 projection;
			glm_mat4_identity(projection);
			glm_perspective(glm_rad(90.0f), (float)1920 / (float)1080, 0.1f, 100.0f, projection);
			setUniformMat4(basicShader.shaderProgram, "projection", projection);
		}
		else {
			mat4 projection;
			glm_mat4_identity(projection);
			glm_perspective(glm_rad(90.0f), (float)windowedWidth / (float)windowedHeight, 0.1f, 100.0f, projection);
			setUniformMat4(basicShader.shaderProgram, "projection", projection);
		}

		setUniformMat4(basicShader.shaderProgram, "view", view);

		for(int i = 0; i < sizeof(pointLightPositions) / sizeof(vec3); i++) {
			//createPointLight(i, basicShader.shaderProgram);
		}
		
		glBindVertexArray(VAO);
		for(int i = 0; i < numOfBlocks; i++) {
			glm_mat4_identity(model);
			glm_translate(model, blocks[i].pos);
			setUniformMat4(basicShader.shaderProgram, "model", model);
			glBindTexture(GL_TEXTURE_2D, textures[DIRT]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapInterval(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(basicShader.shaderProgram);

	glfwTerminate();
	return 0;
}

void setFullscreen(GLFWwindow* window) {
	isFullscreen = !isFullscreen;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if (isFullscreen) {
		// Switch to fullscreen
		glfwGetWindowPos(window, &windowedPosX, &windowedPosY); // Save window position
		glfwGetWindowSize(window, &windowedWidth, &windowedHeight); // Save window size
		glfwSetWindowMonitor(window, monitor, 0, 0, 1920, 1080, mode->refreshRate);
	} else {
		// Switch back to windowed mode
		glfwSetWindowMonitor(window, NULL, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_F11 && action == GLFW_RELEASE) {
		setFullscreen(window);
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

void generateBuffers(unsigned int* VBO, unsigned int* VAO) {
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	xoffset *= cam.sensitivity;
	yoffset *= cam.sensitivity;
	cam.yaw += xoffset;
	cam.pitch += yoffset;
	if (cam.pitch > 89.0f) {cam.pitch = 89.0f;}
    if (cam.pitch < -89.0f) {cam.pitch = -89.0f;}
	vec3 direction;
	direction[0] = cos(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
	direction[1] = sin(glm_rad(cam.pitch));
	direction[2] = sin(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
	glm_normalize(direction);
	glm_vec3_copy(direction, cam.cameraFront);
	
}

void createPointLight(int index, unsigned int shaderProgram) {
    char baseName[50];
	snprintf(baseName, sizeof(baseName), "pointLights[%d]", index);
	
	char fullName[100];

	snprintf(fullName, sizeof(fullName), "%s.position", baseName);
	setVec3(shaderProgram, fullName, pointLightPositions[index]);

	snprintf(fullName, sizeof(fullName), "%s.ambient", baseName);
	setVec3(shaderProgram, fullName, (vec3){0.3f, 0.24f, 0.14f});

	snprintf(fullName, sizeof(fullName), "%s.diffuse", baseName);
	setVec3(shaderProgram, fullName, (vec3){0.7f, 0.5f, 0.3f});

	snprintf(fullName, sizeof(fullName), "%s.specular", baseName);
	setVec3(shaderProgram, fullName, (vec3){1.0f, 0.9f, 0.8f});

	snprintf(fullName, sizeof(fullName), "%s.constant", baseName);
	setFloat(shaderProgram, fullName, 0.18f);

	snprintf(fullName, sizeof(fullName), "%s.linear", baseName);
	setFloat(shaderProgram, fullName, 1.0f);

	snprintf(fullName, sizeof(fullName), "%s.quadratic", baseName);
	setFloat(shaderProgram, fullName, 0.12f);
}
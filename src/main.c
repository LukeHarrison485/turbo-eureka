#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include "shader.h"
#include "camera.h"

#include "perlin.h"

float vertices[] = {
    // Positions          
    // Front face
    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

    // Back face
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f
};

vec3 cubePositions[] = {
	vec3{0.0f, 0.0f, 0.0f},
	vec3{3.0f, 6.0f, 5.0f},
	vec3{5.0f, 2.0f, 0.0f},
	vec3{0.0f, 0.0f, 3.0f},
};

unsigned int indices[] = {
    // Front face
    0, 1, 2, // Triangle 1
    2, 3, 0, // Triangle 2

    // Back face
    4, 5, 6, // Triangle 3
    6, 7, 4, // Triangle 4

    // Left face
    4, 0, 3, // Triangle 5
    3, 7, 4, // Triangle 6

    // Right face
    1, 5, 6, // Triangle 7
    6, 2, 1, // Triangle 8

    // Top face
    3, 2, 6, // Triangle 9
    6, 7, 3, // Triangle 10

    // Bottom face
    4, 5, 1, // Triangle 11
    1, 0, 4  // Triangle 12
};

Camera cam = createCamera(vec3{0.0f, 0.0f, 0.0f}, 2.0f, 45.0f, 0.1f);
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow* window);
void generateBuffers(unsigned int* VBO, unsigned int* VAO, unsigned int* EBO);
unsigned int compileShader(unsigned int type, const char* source);
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

unsigned int SCREENWIDTH = 1920;
unsigned int SCREENHEIGHT = 1080;

float lastX = (float)SCREENWIDTH / 2.0f, lastY = (float)SCREENHEIGHT / 2.0f;

int main(void) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);
	createPerlinNoise(32, 256, 256, 1);
	GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "YEAH!!!!!!!!! BLOCK GAME, THIS IS THE MOST ORGINIAL IDEA I HAVE EVER HAD!!!!!!!!!!!!!", NULL, NULL);
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
	unsigned int EBO = 0;
	generateBuffers(&VBO, &VAO, &EBO);
	Shader basicShader = createShader("shader/basic.vs", "shader/basic.fs");  

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

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
		basicShader.setUniformFloat(basicShader.shaderProgram, "time", time);

		glUseProgram(basicShader.shaderProgram);

		mat4 model;
		glm_mat4_identity(model);
		glm_rotate(model, -1.0f * glfwGetTime(), vec3{1.0f, 0.0f, 0.5f});

		mat4 view;
		glm_mat4_identity(view);
		updateView(cam, view);


		mat4 projection;
		glm_mat4_identity(projection);
		glm_perspective(glm_rad(90.0f), (float)SCREENWIDTH / (float)SCREENHEIGHT, 0.1f, 100.0f, projection);

		setUniformMat4(basicShader.shaderProgram, "view", view);
		setUniformMat4(basicShader.shaderProgram, "projection", projection);

		glBindVertexArray(VAO);
		for(int i = 0; i < 4; i++) {
			glm_mat4_identity(model);
			glm_translate(model, cubePositions[i]);
			if(i == 2) {
				glm_scale(model, vec3{.5, .5, .5});
			}
			setUniformMat4(basicShader.shaderProgram, "model", model);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

void generateBuffers(unsigned int* VBO, unsigned int* VAO, unsigned int* EBO) {
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
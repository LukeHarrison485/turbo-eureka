#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include "shader.h"
#include "camera.h"
#include <string.h>
#include "block.h"

typedef struct 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
	vec3 position;
} pointLight;


pointLight pointLightArr[100];
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
void createPointLight(vec3 pos, vec3 amb, vec3 diff, vec3 spec, float constant, float lin, float quad);
void createDirLight(unsigned int shaderProgram, vec3 dir, vec3 ambient, vec3 spec, vec3 diff);
void pointLightShaderPassthrough(unsigned int shaderProgram);

int isFullscreen = 0;
int windowedWidth = 800; 
int windowedHeight = 600; 
int windowedPosX = 0;
int windowedPosY = 0;
int numOfPointLights = 0;
int wireFrame = 0;

float lastX;
float lastY;

int main(void) {
	cam = createCamera((vec3){0.0f, 50.0f, 0.0f} , 12.0f, 45.0f, 0.1f);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);
	lastX = (float)windowedWidth / 2.0f;
	lastY = (float)windowedHeight / 2.0f;
	GLFWwindow* window = glfwCreateWindow(windowedWidth, windowedHeight, "YEAH!!!!!!!!! BLOCK GAME, THIS IS THE MOST ORGINIAL IDEA I HAVE EVER HAD!!!!!!!!!!!!!", NULL, NULL);
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
	unsigned int InstanceBuffer;
	generateBuffers(&VBO, &VAO);
	
	Shader basicShader = createShader("shader/basic.vs", "shader/basic.fs");  

	block* blocks = createChunk(300, 1);
	printf("Number of cubes generated: %d\n", numOfBlocks);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glfwSetKeyCallback(window, keyCallback);

	textures[DIRT] = loadTexture("dirt.png");
	glUniform1i(glGetUniformLocation(basicShader.shaderProgram, "material.diffuse"), textures[DIRT]);
	glUniform1i(glGetUniformLocation(basicShader.shaderProgram, "material.specular"), textures[DIRT]);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	

	mat4* models = (mat4*)malloc(numOfBlocks * sizeof(mat4));

	for(int i = 0; i < numOfBlocks; i++) {
		mat4 model;
		glm_mat4_identity(model);
		glm_translate(model, blocks[i].pos);
		glm_mat4_copy(model, models[i]);
	}
	
	unsigned int instanceBuffer;
	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, numOfBlocks * sizeof(mat4), models, GL_STATIC_DRAW);

	// Ensure the VAO is bound before setting attributes
	glBindVertexArray(VAO);

	// Update the vertex attributes for the instance buffer (if not already done in generateBuffers)
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	// Mat4 is split into 4 vec4 attributes
	GLsizei vec4Size = sizeof(vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(uintptr_t)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(uintptr_t)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(uintptr_t)(3 * vec4Size));

	// Set divisors for instanced attributes
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);


	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame; 

		processInput(window);
		processCameraInput(window, &cam, deltaTime);
		
		
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(basicShader.shaderProgram);

		// Set view position (camera position)
		glUniform3fv(glGetUniformLocation(basicShader.shaderProgram, "viewPos"), 1, cam.cameraPos);
		createDirLight(basicShader.shaderProgram, (vec3){-0.2f, -1.0f, -0.3f}, (vec3){0.7f, 0.7f, 0.7f}, (vec3){0.4f, 0.4f, 0.4f}, (vec3){0.5f, 0.5f, 0.5f});
		glUniform1f(glGetUniformLocation(basicShader.shaderProgram, "material.shininess"), 1.0f);
		glUniform1i(glGetUniformLocation(basicShader.shaderProgram, "numPointLights"), numOfPointLights);
		pointLightShaderPassthrough(basicShader.shaderProgram);
		glUniform3fv(glGetUniformLocation(basicShader.shaderProgram, "planetCenter"), 1, (vec3){50.0f, 0.0f, 50.0f});

		mat4 view;
		glm_mat4_identity(view);
		updateView(cam, view);

		if (isFullscreen) {
			glViewport(0, 0, 1920, 1080); // Update viewport
			// Update projection matrix (if needed)
			mat4 projection;
			glm_mat4_identity(projection);
			glm_perspective(glm_rad(90.0f), (float)1920 / (float)1080, 0.1f, 1000.0f, projection);
			setUniformMat4(basicShader.shaderProgram, "projection", projection);
		}
		else {
			mat4 projection;
			glm_mat4_identity(projection);
			glm_perspective(glm_rad(90.0f), (float)windowedWidth / (float)windowedHeight, 0.1f, 1000.0f, projection);
			setUniformMat4(basicShader.shaderProgram, "projection", projection);
		}

		setUniformMat4(basicShader.shaderProgram, "view", view);

		glBindVertexArray(VAO);
		
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, numOfBlocks);
		

		glfwSwapInterval(1);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(basicShader.shaderProgram);
	free(blocks);

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
	if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
		createPointLight(cam.cameraPos, (vec3){1.0f, 1.0f, 1.0f}, (vec3){0.8f, 0.8f, 0.8f}, (vec3){1.0f, 1.0f, 1.0f}, 1.0f, 0.09f, 0.032f);
	}
	if (key == GLFW_KEY_G && action == GLFW_RELEASE) {
		if(wireFrame) {
			wireFrame = 0;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			wireFrame = 1;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
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

void createPointLight(vec3 pos, vec3 amb, vec3 diff, vec3 spec, float constant, float lin, float quad) {
	glm_vec3_copy(pos, pointLightArr[numOfPointLights].position);
	glm_vec3_copy(amb, pointLightArr[numOfPointLights].ambient);
	glm_vec3_copy(diff, pointLightArr[numOfPointLights].diffuse);
	glm_vec3_copy(spec, pointLightArr[numOfPointLights].specular);
	pointLightArr[numOfPointLights].constant = constant;
	pointLightArr[numOfPointLights].linear = lin;
	pointLightArr[numOfPointLights].quadratic = quad;
	numOfPointLights++;
}

void createDirLight(unsigned int shaderProgram, vec3 dir, vec3 ambient, vec3 spec, vec3 diff) {
	glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.direction"), 1, dir);
	glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 1, ambient);
	glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.specular"), 1, spec);
	glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 1, diff);
}

void pointLightShaderPassthrough(unsigned int shaderProgram) {
	char baseName[50];
	char fullName[100];
	for(int i = 0; i < numOfPointLights; i++) {
		snprintf(baseName, sizeof(baseName), "pointLights[%d]", i);

		snprintf(fullName, sizeof(fullName), "%s.position", baseName);
		setVec3(shaderProgram, fullName, pointLightArr[i].position);

		snprintf(fullName, sizeof(fullName), "%s.ambient", baseName);
		setVec3(shaderProgram, fullName, pointLightArr[i].ambient);

		snprintf(fullName, sizeof(fullName), "%s.diffuse", baseName);
		setVec3(shaderProgram, fullName, pointLightArr[i].diffuse);

		snprintf(fullName, sizeof(fullName), "%s.specular", baseName);
		setVec3(shaderProgram, fullName, pointLightArr[i].specular);

		snprintf(fullName, sizeof(fullName), "%s.constant", baseName);
		setFloat(shaderProgram, fullName, pointLightArr[i].constant);

		snprintf(fullName, sizeof(fullName), "%s.linear", baseName);
		setFloat(shaderProgram, fullName, pointLightArr[i].linear);

		snprintf(fullName, sizeof(fullName), "%s.quadratic", baseName);
		setFloat(shaderProgram, fullName, pointLightArr[i].quadratic);
	}
}
/*


*/
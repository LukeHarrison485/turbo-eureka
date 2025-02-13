#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>

void setFloat(unsigned int shaderProgram, const char* location, float val) {
    int uniformLocation = glGetUniformLocation(shaderProgram, location);
	glUniform1f(uniformLocation, val);
}

void setVec3(unsigned int shaderProgram, const char* location, vec3 val) {
    int uniformLocation = glGetUniformLocation(shaderProgram, location);
	glUniform3f(uniformLocation, val[0], val[1], val[2]);
}

void setUniformMat4(unsigned int shaderProgram, const char* location, mat4 val) {
	int uniformLocation = glGetUniformLocation(shaderProgram, location);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float*)val);
}

typedef struct Shader {
    unsigned int shaderProgram;
    void (*setFloat)(unsigned int, const char*, float);
	void (*setUniformMat4)(unsigned int, const char*, mat4*);
} Shader;

char* readShaderSource(const char* path) {
	FILE* file = fopen(path, "rb");
	if (!file) {
		fprintf_s(stderr, "Error when opening file at %s\n", path);
		return NULL;
	}
	fseek(file, 0l, SEEK_END);
	long size = ftell(file);
	fseek(file, 0l, SEEK_SET);
	char* content = (char*)malloc(size + 1);
	if (!content) {
		fprintf(stderr, "Error when allocating memory during shader reading\n");
		fclose(file);
		return NULL;
	}

	size_t bytesRead = fread(content, 1, size, file);
	if (bytesRead != size) {
		fprintf(stderr, "Failed to read shader source at: %s\n", path);
		free(content);
		fclose(file);
		return NULL;
	}
	content[size] = '\0';
	fclose(file);
	return content;
}

unsigned int compileShader(unsigned int type, const char* path) {
	unsigned int shader = glCreateShader(type);
	char* content = readShaderSource(path);
	if (!content) {
		return 0;
	}
	glShaderSource(shader, 1, (const GLchar* const*)&content, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
	}

	return shader;
}

Shader createShader(const char* vertexSource, const char* fragmentSource) {
    Shader shader;
    shader.shaderProgram = 0; // Initialize to an invalid value

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) {
        fprintf(stderr, "Failed to compile vertex shader.\n");
        return shader; // Return an invalid Shader object
    }

    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0) {
        fprintf(stderr, "Failed to compile fragment shader.\n");
        glDeleteShader(vertexShader);
        return shader; // Return an invalid Shader object
    }

    shader.shaderProgram = glCreateProgram();
    glAttachShader(shader.shaderProgram, vertexShader);
    glAttachShader(shader.shaderProgram, fragmentShader);
    glLinkProgram(shader.shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shader.shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader.shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        glDeleteProgram(shader.shaderProgram);
        shader.shaderProgram = 0; // Mark as invalid
    }

    // Validate the program (optional but recommended)
    glValidateProgram(shader.shaderProgram);
    glGetProgramiv(shader.shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader.shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::VALIDATION_FAILED\n%s\n", infoLog);
        glDeleteProgram(shader.shaderProgram);
        shader.shaderProgram = 0; // Mark as invalid
    }

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Initialize the function pointer
    shader.setFloat = setFloat;

    return shader;
}
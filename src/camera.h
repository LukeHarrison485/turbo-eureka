#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>

typedef struct {
    vec3 cameraPos;
    vec3 cameraFront;
    vec3 cameraUp;
    float speed;
    float fov;
    float sensitivity;
    float pitch;
    float yaw;
    
} Camera;

Camera createCamera(vec3 pos, float speed, float fov, float sens) {
    Camera cam;
    glm_vec3_copy(pos, cam.cameraPos);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, cam.cameraUp); // Initialize cameraUp
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, cam.cameraFront); // Default cameraFront
    cam.fov = fov;
    cam.speed = speed;
    cam.sensitivity = sens;
    cam.yaw = -90.0f;
    return cam;
}

void updateView(Camera cam, mat4 view) {
    vec3 target;
    glm_vec3_add(cam.cameraPos, cam.cameraFront, target); // Calculate target point
    glm_lookat(cam.cameraPos, target, cam.cameraUp, view); // Create view matrix
}

void processCameraInput(GLFWwindow* window, Camera* cam, float deltaTime) {
    vec3 temp;
    float velocity = cam->speed * deltaTime; // Scale speed by deltaTime

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_scale(cam->cameraFront, velocity, temp);
        glm_vec3_add(cam->cameraPos, temp, cam->cameraPos); // Move forward
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_scale(cam->cameraFront, velocity, temp);
        glm_vec3_sub(cam->cameraPos, temp, cam->cameraPos); // Move backward
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_cross(cam->cameraFront, cam->cameraUp, temp); // Calculate right vector
        glm_normalize(temp);
        glm_vec3_scale(temp, velocity, temp);
        glm_vec3_sub(cam->cameraPos, temp, cam->cameraPos); // Move left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_cross(cam->cameraFront, cam->cameraUp, temp); // Calculate right vector
        glm_normalize(temp);
        glm_vec3_scale(temp, velocity, temp);
        glm_vec3_add(cam->cameraPos, temp, cam->cameraPos); // Move right
    }
}
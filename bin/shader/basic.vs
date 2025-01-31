#version 460 core
layout (location = 0) in vec3 aPos; // Input vertex position

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transform the vertex position to clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
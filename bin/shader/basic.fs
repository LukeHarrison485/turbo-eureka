#version 460 core
out vec4 FragColor; // Output color of the fragment

uniform float time;

void main()
{
    // Set the fragment color to orange
    float speed = 2.0; // Adjust this value to control the speed
    float intensity = 0.8; // Adjust this value to control the intensity
    float red = 0.5f + intensity * 0.5f * sin(time * speed);
    float green = 0.5f + intensity * 0.5f * sin(time * speed + 2.0f);
    float blue = 0.5f + intensity * 0.5f * sin(time * speed + 4.0f);
    float shift = 0.5f + intensity * 0.5f * sin(time * speed);
    FragColor = vec4(red, green, blue, 1.0);
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0f / 2.2));
}
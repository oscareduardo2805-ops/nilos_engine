#version 330 core

// Input from vertex shader
in vec3 FragColor;

// Output color
out vec4 FinalColor;

void main() {
    // Simple pass-through shader with vertex colors
    FinalColor = vec4(FragColor, 1.0);
}


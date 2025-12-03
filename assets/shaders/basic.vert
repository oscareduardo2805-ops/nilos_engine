#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Output to fragment shader
out vec3 FragColor;

// Uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    // Transform vertex position
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    
    // Pass color to fragment shader
    FragColor = aColor;
}


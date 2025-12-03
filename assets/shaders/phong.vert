#version 330 core

// Input attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

// Output to fragment shader
out vec3 FragPos;
out vec3 Normal;
out vec3 VertexColor;
out vec2 TexCoord;

// Uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;

void main() {
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = uNormalMatrix * aNormal;
    VertexColor = aColor;
    TexCoord = aTexCoord;
    
    gl_Position = uProjection * uView * vec4(FragPos, 1.0);
}


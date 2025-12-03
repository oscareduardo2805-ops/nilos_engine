#version 330 core

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec3 VertexColor;
in vec2 TexCoord;

// Output
out vec4 FragColor;

// Material properties
uniform vec3 uMaterialDiffuse;
uniform vec3 uMaterialSpecular;
uniform float uMaterialShininess;

// Lighting
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform float uLightIntensity;
uniform vec3 uAmbientLight;
uniform vec3 uViewPos;

// Textures (optional)
uniform sampler2D uDiffuseMap;
uniform bool uUseDiffuseMap;

void main() {
    // Normalize normal
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-uLightDir);
    
    // Ambient
    vec3 ambient = uAmbientLight;
    
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor * uLightIntensity;
    
    // Specular (Blinn-Phong for better performance)
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), uMaterialShininess);
    vec3 specular = spec * uMaterialSpecular * uLightColor;
    
    // Get base color
    vec3 baseColor = uUseDiffuseMap ? texture(uDiffuseMap, TexCoord).rgb : VertexColor;
    
    // Combine
    vec3 result = (ambient + diffuse) * baseColor * uMaterialDiffuse + specular;
    
    FragColor = vec4(result, 1.0);
}


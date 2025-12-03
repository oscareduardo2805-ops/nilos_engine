#include "Shader.h"
#include "../Core/Logger.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace Nilos {

Shader::~Shader() {
    Delete();
}

bool Shader::LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    // Read vertex shader
    std::ifstream vertexFile(vertexPath);
    if (!vertexFile.is_open()) {
        NILOS_ERROR("Failed to open vertex shader file: ", vertexPath);
        return false;
    }
    std::stringstream vertexStream;
    vertexStream << vertexFile.rdbuf();
    std::string vertexSource = vertexStream.str();
    vertexFile.close();

    // Read fragment shader
    std::ifstream fragmentFile(fragmentPath);
    if (!fragmentFile.is_open()) {
        NILOS_ERROR("Failed to open fragment shader file: ", fragmentPath);
        return false;
    }
    std::stringstream fragmentStream;
    fragmentStream << fragmentFile.rdbuf();
    std::string fragmentSource = fragmentStream.str();
    fragmentFile.close();

    return LoadFromSource(vertexSource, fragmentSource);
}

bool Shader::LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    // Compile vertex shader
    uint32_t vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        return false;
    }

    // Compile fragment shader
    uint32_t fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return false;
    }

    // Link program
    bool success = LinkProgram(vertexShader, fragmentShader);

    // Clean up shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return success;
}

void Shader::Use() const {
    if (m_ProgramId != 0) {
        glUseProgram(m_ProgramId);
    }
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::Delete() {
    if (m_ProgramId != 0) {
        glDeleteProgram(m_ProgramId);
        m_ProgramId = 0;
    }
}

uint32_t Shader::CompileShader(const std::string& source, uint32_t type) {
    uint32_t shader = glCreateShader(type);
    const char* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    // Check compilation
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        const char* shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        NILOS_ERROR("Shader compilation failed (", shaderType, "): ", infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Shader::LinkProgram(uint32_t vertexShader, uint32_t fragmentShader) {
    m_ProgramId = glCreateProgram();
    glAttachShader(m_ProgramId, vertexShader);
    glAttachShader(m_ProgramId, fragmentShader);
    glLinkProgram(m_ProgramId);

    // Check linking
    int success;
    glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_ProgramId, 512, nullptr, infoLog);
        NILOS_ERROR("Shader linking failed: ", infoLog);
        glDeleteProgram(m_ProgramId);
        m_ProgramId = 0;
        return false;
    }

    return true;
}

int Shader::GetUniformLocation(const std::string& name) const {
    // Simple caching could be added here for performance
    return glGetUniformLocation(m_ProgramId, name.c_str());
}

// ============================================================================
// Uniform Setters
// ============================================================================

void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

} // namespace Nilos


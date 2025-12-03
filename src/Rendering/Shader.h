#pragma once

#include <string>
#include <glm/glm.hpp>
#include <cstdint>

namespace Nilos {

/**
 * @brief Shader program management
 * 
 * Loads, compiles, and links vertex and fragment shaders.
 * Provides utilities to set uniform values.
 */
class Shader {
public:
    Shader() = default;
    ~Shader();

    /**
     * @brief Load and compile shader from file paths
     */
    bool LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

    /**
     * @brief Load and compile shader from source strings
     */
    bool LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource);

    /**
     * @brief Use/activate this shader program
     */
    void Use() const;

    /**
     * @brief Unbind shader
     */
    void Unbind() const;

    /**
     * @brief Delete shader program
     */
    void Delete();

    /**
     * @brief Check if shader is valid
     */
    bool IsValid() const { return m_ProgramId != 0; }

    /**
     * @brief Get OpenGL program ID
     */
    uint32_t GetProgramId() const { return m_ProgramId; }

    // ========================================================================
    // Uniform Setters
    // ========================================================================

    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat3(const std::string& name, const glm::mat3& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;

private:
    /**
     * @brief Compile a shader stage
     */
    uint32_t CompileShader(const std::string& source, uint32_t type);

    /**
     * @brief Link vertex and fragment shaders into program
     */
    bool LinkProgram(uint32_t vertexShader, uint32_t fragmentShader);

    /**
     * @brief Get uniform location (cached)
     */
    int GetUniformLocation(const std::string& name) const;

    uint32_t m_ProgramId = 0;
};

} // namespace Nilos


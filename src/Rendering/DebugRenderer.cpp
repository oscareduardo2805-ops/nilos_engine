#include "DebugRenderer.h"
#include "../Core/Logger.h"
#include <glad/glad.h>

namespace Nilos {

bool DebugRenderer::Initialize() {
    // Create simple line shader
    m_Shader = std::make_unique<Shader>();
    
    std::string vertSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        
        out vec3 Color;
        
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        void main() {
            Color = aColor;
            gl_Position = uProjection * uView * vec4(aPos, 1.0);
        }
    )";
    
    std::string fragSrc = R"(
        #version 330 core
        in vec3 Color;
        out vec4 FragColor;
        
        void main() {
            FragColor = vec4(Color, 1.0);
        }
    )";
    
    if (!m_Shader->LoadFromSource(vertSrc, fragSrc)) {
        NILOS_ERROR("Failed to create debug shader");
        return false;
    }
    
    // Create VAO/VBO for lines
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    NILOS_INFO("Debug renderer initialized");
    return true;
}

void DebugRenderer::DrawAABB(const AABB& aabb, const glm::vec3& color) {
    // Draw 12 edges of the box
    glm::vec3 corners[8] = {
        {aabb.Min.x, aabb.Min.y, aabb.Min.z},
        {aabb.Max.x, aabb.Min.y, aabb.Min.z},
        {aabb.Max.x, aabb.Max.y, aabb.Min.z},
        {aabb.Min.x, aabb.Max.y, aabb.Min.z},
        {aabb.Min.x, aabb.Min.y, aabb.Max.z},
        {aabb.Max.x, aabb.Min.y, aabb.Max.z},
        {aabb.Max.x, aabb.Max.y, aabb.Max.z},
        {aabb.Min.x, aabb.Max.y, aabb.Max.z}
    };
    
    // Bottom face
    DrawLine(corners[0], corners[1], color);
    DrawLine(corners[1], corners[2], color);
    DrawLine(corners[2], corners[3], color);
    DrawLine(corners[3], corners[0], color);
    
    // Top face
    DrawLine(corners[4], corners[5], color);
    DrawLine(corners[5], corners[6], color);
    DrawLine(corners[6], corners[7], color);
    DrawLine(corners[7], corners[4], color);
    
    // Vertical edges
    DrawLine(corners[0], corners[4], color);
    DrawLine(corners[1], corners[5], color);
    DrawLine(corners[2], corners[6], color);
    DrawLine(corners[3], corners[7], color);
}

void DebugRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) {
    m_Lines.push_back({start, end, color});
}

void DebugRenderer::DrawPath(const std::vector<glm::vec3>& path, const glm::vec3& color) {
    for (size_t i = 0; i < path.size() - 1; ++i) {
        DrawLine(path[i], path[i + 1], color);
    }
}

void DebugRenderer::Render(const glm::mat4& view, const glm::mat4& projection) {
    if (m_Lines.empty()) return;
    
    m_Shader->Use();
    m_Shader->SetMat4("uView", view);
    m_Shader->SetMat4("uProjection", projection);
    
    // Prepare vertex data (position + color interleaved)
    std::vector<float> vertices;
    vertices.reserve(m_Lines.size() * 12); // 2 vertices * 6 floats per line
    
    for (const auto& line : m_Lines) {
        // Start vertex
        vertices.push_back(line.Start.x);
        vertices.push_back(line.Start.y);
        vertices.push_back(line.Start.z);
        vertices.push_back(line.Color.r);
        vertices.push_back(line.Color.g);
        vertices.push_back(line.Color.b);
        
        // End vertex
        vertices.push_back(line.End.x);
        vertices.push_back(line.End.y);
        vertices.push_back(line.End.z);
        vertices.push_back(line.Color.r);
        vertices.push_back(line.Color.g);
        vertices.push_back(line.Color.b);
    }
    
    // Upload to GPU
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Draw lines
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_Lines.size() * 2));
    
    glBindVertexArray(0);
}

void DebugRenderer::Clear() {
    m_Lines.clear();
}

void DebugRenderer::Shutdown() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    NILOS_INFO("Debug renderer shutdown");
}

} // namespace Nilos


#include "Renderer.h"
#include "../Core/Logger.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Nilos {

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Initialize() {
    NILOS_INFO("Initializing renderer...");

    // Set clear color (nice blue-gray)
    m_ClearColor = glm::vec4(0.1f, 0.15f, 0.2f, 1.0f);

    // Create basic shader
    m_BasicShader = std::make_unique<Shader>();
    
    // Basic shader source (embedded for simplicity, will be moved to files)
    std::string vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        
        out vec3 FragColor;
        
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        void main() {
            gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
            FragColor = aColor;
        }
    )";
    
    std::string fragmentSource = R"(
        #version 330 core
        in vec3 FragColor;
        out vec4 FinalColor;
        
        void main() {
            FinalColor = vec4(FragColor, 1.0);
        }
    )";
    
    if (!m_BasicShader->LoadFromSource(vertexSource, fragmentSource)) {
        NILOS_ERROR("Failed to create basic shader");
        return false;
    }

    NILOS_INFO("Renderer initialized successfully");
    return true;
}

void Renderer::Shutdown() {
    if (m_BasicShader) {
        m_BasicShader->Delete();
        m_BasicShader.reset();
    }
    NILOS_INFO("Renderer shutdown");
}

void Renderer::BeginFrame() {
    // Clear buffers
    glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame() {
    // Nothing to do here for now
    // Future: flush render queue, finalize post-processing, etc.
}

void Renderer::RenderMesh(MeshComponent& mesh, 
                          const TransformComponent& transform,
                          const CameraComponent& camera,
                          const TransformComponent& cameraTransform) {
    // Initialize mesh buffers if not done yet
    if (!mesh.IsInitialized()) {
        InitializeMeshBuffers(mesh);
    }

    // Use shader
    m_BasicShader->Use();

    // Set matrices
    glm::mat4 model = transform.GetModelMatrix();
    glm::mat4 view = camera.GetViewMatrix(cameraTransform.Position);
    glm::mat4 projection = camera.ProjectionMatrix;

    m_BasicShader->SetMat4("uModel", model);
    m_BasicShader->SetMat4("uView", view);
    m_BasicShader->SetMat4("uProjection", projection);

    // Bind VAO and draw
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.Indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    m_ClearColor = glm::vec4(r, g, b, a);
}

void Renderer::InitializeMeshBuffers(MeshComponent& mesh) {
    // Generate buffers
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    // Bind VAO
    glBindVertexArray(mesh.VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 mesh.Vertices.size() * sizeof(float), 
                 mesh.Vertices.data(), 
                 GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 mesh.Indices.size() * sizeof(uint32_t), 
                 mesh.Indices.data(), 
                 GL_STATIC_DRAW);

    // Configure vertex attributes
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind
    glBindVertexArray(0);

    NILOS_DEBUG("Mesh buffers initialized (VAO: ", mesh.VAO, ")");
}

} // namespace Nilos


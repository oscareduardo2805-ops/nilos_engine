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

    // Set clear color
    m_ClearColor = glm::vec4(0.1f, 0.15f, 0.2f, 1.0f);

    // Load Phong shader
    m_PhongShader = std::make_unique<Shader>();
    if (!m_PhongShader->LoadFromFiles("assets/shaders/phong.vert", "assets/shaders/phong.frag")) {
        NILOS_ERROR("Failed to load Phong shader");
        return false;
    }

    // Setup default lighting (will be overridden by scene)
    m_DirectionalLight.Direction = glm::normalize(glm::vec3(-1.0f, -1.0f, -0.5f));
    m_DirectionalLight.Color = glm::vec3(1.0f, 0.95f, 0.9f);
    m_DirectionalLight.Intensity = 1.2f;

    m_AmbientLight.Color = glm::vec3(0.15f, 0.2f, 0.25f);
    m_AmbientLight.Intensity = 0.3f;

    NILOS_INFO("Renderer initialized successfully");
    return true;
}

void Renderer::Shutdown() {
    if (m_PhongShader) {
        m_PhongShader->Delete();
        m_PhongShader.reset();
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
    m_PhongShader->Use();

    // Set matrices
    glm::mat4 model = transform.GetModelMatrix();
    glm::mat4 view = camera.GetViewMatrix(cameraTransform.Position);
    glm::mat4 projection = camera.ProjectionMatrix;
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

    m_PhongShader->SetMat4("uModel", model);
    m_PhongShader->SetMat4("uView", view);
    m_PhongShader->SetMat4("uProjection", projection);
    m_PhongShader->SetMat3("uNormalMatrix", normalMatrix);

    // Set material (simple default)
    m_PhongShader->SetVec3("uMaterialDiffuse", glm::vec3(1.0f));
    m_PhongShader->SetVec3("uMaterialSpecular", glm::vec3(0.3f));
    m_PhongShader->SetFloat("uMaterialShininess", 32.0f);
    m_PhongShader->SetInt("uUseDiffuseMap", 0);

    // Set lighting
    m_PhongShader->SetVec3("uLightDir", m_DirectionalLight.Direction);
    m_PhongShader->SetVec3("uLightColor", m_DirectionalLight.Color);
    m_PhongShader->SetFloat("uLightIntensity", m_DirectionalLight.Intensity);
    m_PhongShader->SetVec3("uAmbientLight", m_AmbientLight.Color * m_AmbientLight.Intensity);
    m_PhongShader->SetVec3("uViewPos", cameraTransform.Position);

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

    // Vertex format: Position(3) + Normal(3) + Color(3) + TexCoord(2) = 11 floats
    size_t stride = 11 * sizeof(float);

    // Position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Normal (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Color (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // TexCoord (location = 3)
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // Unbind
    glBindVertexArray(0);

    NILOS_DEBUG("Mesh buffers initialized (VAO: ", mesh.VAO, ")");
}

} // namespace Nilos


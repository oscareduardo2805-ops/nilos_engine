#include "Texture.h"
#include "../Core/Logger.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb/stb_image.h"

namespace Nilos {

Texture2D::~Texture2D() {
    Delete();
}

bool Texture2D::LoadFromFile(const std::string& filepath, bool generateMipmaps) {
    // Flip textures on load (OpenGL expects bottom-left origin)
    stbi_set_flip_vertically_on_load(true);

    // Load image
    unsigned char* data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);
    if (!data) {
        NILOS_ERROR("Failed to load texture: ", filepath);
        NILOS_ERROR("Reason: ", stbi_failure_reason());
        return false;
    }

    // Determine format
    TextureFormat format = TextureFormat::RGB;
    if (m_Channels == 4) format = TextureFormat::RGBA;
    else if (m_Channels == 3) format = TextureFormat::RGB;
    else if (m_Channels == 1) format = TextureFormat::RGB; // Grayscale

    m_Format = format;
    m_Filepath = filepath;

    // Create from data
    bool success = CreateFromData(data, m_Width, m_Height, format, generateMipmaps);

    // Free image data
    stbi_image_free(data);

    if (success) {
        NILOS_DEBUG("Texture loaded: ", filepath, " (", m_Width, "x", m_Height, ", ", m_Channels, " channels)");
    }

    return success;
}

bool Texture2D::CreateFromData(const unsigned char* data, int width, int height, 
                                TextureFormat format, bool generateMipmaps) {
    m_Width = width;
    m_Height = height;
    m_Format = format;

    // Generate texture
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // Get OpenGL formats
    uint32_t internalFormat, dataFormat, dataType;
    FormatToGL(format, internalFormat, dataFormat, dataType);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, 
                 dataFormat, dataType, data);

    // Generate mipmaps if requested
    if (generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
        SetFilter(TextureFilter::LinearMipmapLinear, TextureFilter::Linear);
    } else {
        SetFilter(TextureFilter::Linear, TextureFilter::Linear);
    }

    // Set default wrap mode
    SetWrap(TextureWrap::Repeat, TextureWrap::Repeat);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture2D::Bind(uint32_t unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture2D::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Delete() {
    if (m_TextureID != 0) {
        glDeleteTextures(1, &m_TextureID);
        m_TextureID = 0;
    }
}

void Texture2D::SetFilter(TextureFilter minFilter, TextureFilter magFilter) {
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterToGL(minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterToGL(magFilter));
}

void Texture2D::SetWrap(TextureWrap wrapS, TextureWrap wrapT) {
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL(wrapT));
}

uint32_t Texture2D::FilterToGL(TextureFilter filter) {
    switch (filter) {
        case TextureFilter::Nearest: return GL_NEAREST;
        case TextureFilter::Linear: return GL_LINEAR;
        case TextureFilter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilter::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
        case TextureFilter::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
        case TextureFilter::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
        default: return GL_LINEAR;
    }
}

uint32_t Texture2D::WrapToGL(TextureWrap wrap) {
    switch (wrap) {
        case TextureWrap::Repeat: return GL_REPEAT;
        case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
        case TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
        default: return GL_REPEAT;
    }
}

void Texture2D::FormatToGL(TextureFormat format, uint32_t& internalFormat, 
                           uint32_t& dataFormat, uint32_t& dataType) {
    dataType = GL_UNSIGNED_BYTE;
    
    switch (format) {
        case TextureFormat::RGB:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            break;
        case TextureFormat::RGBA:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
        case TextureFormat::DepthComponent:
            internalFormat = GL_DEPTH_COMPONENT;
            dataFormat = GL_DEPTH_COMPONENT;
            dataType = GL_FLOAT;
            break;
        case TextureFormat::DepthStencil:
            internalFormat = GL_DEPTH24_STENCIL8;
            dataFormat = GL_DEPTH_STENCIL;
            dataType = GL_UNSIGNED_INT_24_8;
            break;
    }
}

// ============================================================================
// TextureManager
// ============================================================================

Texture2D* TextureManager::Load(const std::string& filepath, bool generateMipmaps) {
    // Check if already loaded
    auto it = m_Textures.find(filepath);
    if (it != m_Textures.end()) {
        return it->second.get();
    }

    // Load new texture
    auto texture = std::make_unique<Texture2D>();
    if (!texture->LoadFromFile(filepath, generateMipmaps)) {
        return nullptr;
    }

    Texture2D* ptr = texture.get();
    m_Textures[filepath] = std::move(texture);
    return ptr;
}

Texture2D* TextureManager::GetTexture(const std::string& filepath) {
    auto it = m_Textures.find(filepath);
    return (it != m_Textures.end()) ? it->second.get() : nullptr;
}

void TextureManager::Clear() {
    m_Textures.clear();
    NILOS_INFO("All textures unloaded");
}

void TextureManager::Unload(const std::string& filepath) {
    m_Textures.erase(filepath);
}

} // namespace Nilos


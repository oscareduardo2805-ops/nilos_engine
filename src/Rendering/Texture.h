#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>

namespace Nilos {

/**
 * @brief Texture filtering modes
 */
enum class TextureFilter {
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};

/**
 * @brief Texture wrapping modes
 */
enum class TextureWrap {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

/**
 * @brief Texture format
 */
enum class TextureFormat {
    RGB,
    RGBA,
    DepthComponent,
    DepthStencil
};

/**
 * @brief Texture2D class for managing 2D textures
 * 
 * Handles loading, uploading to GPU, and binding textures.
 * Supports various filtering and wrapping modes.
 */
class Texture2D {
public:
    Texture2D() = default;
    ~Texture2D();

    /**
     * @brief Load texture from file
     * @param filepath Path to image file (PNG, JPG, TGA, etc.)
     * @param generateMipmaps Whether to generate mipmaps
     * @return True if loaded successfully
     */
    bool LoadFromFile(const std::string& filepath, bool generateMipmaps = true);

    /**
     * @brief Create texture from raw data
     * @param data Pointer to pixel data
     * @param width Texture width
     * @param height Texture height
     * @param format Texture format
     * @param generateMipmaps Whether to generate mipmaps
     */
    bool CreateFromData(const unsigned char* data, int width, int height, 
                       TextureFormat format, bool generateMipmaps = true);

    /**
     * @brief Bind texture to a texture unit
     * @param unit Texture unit (0-31)
     */
    void Bind(uint32_t unit = 0) const;

    /**
     * @brief Unbind texture
     */
    void Unbind() const;

    /**
     * @brief Delete texture from GPU
     */
    void Delete();

    /**
     * @brief Set texture filtering mode
     */
    void SetFilter(TextureFilter minFilter, TextureFilter magFilter);

    /**
     * @brief Set texture wrapping mode
     */
    void SetWrap(TextureWrap wrapS, TextureWrap wrapT);

    // Getters
    uint32_t GetID() const { return m_TextureID; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    bool IsValid() const { return m_TextureID != 0; }
    const std::string& GetFilepath() const { return m_Filepath; }

private:
    uint32_t m_TextureID = 0;
    int m_Width = 0;
    int m_Height = 0;
    int m_Channels = 0;
    TextureFormat m_Format = TextureFormat::RGBA;
    std::string m_Filepath;

    /**
     * @brief Convert TextureFilter enum to OpenGL constant
     */
    static uint32_t FilterToGL(TextureFilter filter);

    /**
     * @brief Convert TextureWrap enum to OpenGL constant
     */
    static uint32_t WrapToGL(TextureWrap wrap);

    /**
     * @brief Convert TextureFormat enum to OpenGL constants
     */
    static void FormatToGL(TextureFormat format, uint32_t& internalFormat, 
                          uint32_t& dataFormat, uint32_t& dataType);
};

/**
 * @brief Texture manager for caching and reusing textures
 * 
 * Prevents loading the same texture multiple times.
 */
class TextureManager {
public:
    static TextureManager& Get() {
        static TextureManager instance;
        return instance;
    }

    /**
     * @brief Load texture from file (cached)
     * @return Pointer to texture, or nullptr if failed
     */
    Texture2D* Load(const std::string& filepath, bool generateMipmaps = true);

    /**
     * @brief Get cached texture by filepath
     */
    Texture2D* GetTexture(const std::string& filepath);

    /**
     * @brief Unload all textures
     */
    void Clear();

    /**
     * @brief Unload specific texture
     */
    void Unload(const std::string& filepath);

private:
    TextureManager() = default;
    std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_Textures;
};

} // namespace Nilos


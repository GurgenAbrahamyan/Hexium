#include "TextureManager.h"
#include "../stb/stb_image.h"
#include <iostream>
#include "../../core/Event.h"
#include "../../core/EventBus.h"

TextureID TextureManager::addTexture(const std::string& path)
{
    TextureKey key{ path };

    
    if (auto it = lookup.find(key); it != lookup.end()) {
        std::cout << "Texture already loaded: " << path << " (ID: " << it->second << ")\n";
        return it->second;
    }

    int width = 0, height = 0, channels = 0;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        std::cerr << "Failed to load texture: " << path << "\n";
        return UINT32_MAX;
    }

    std::cout << "Loading texture: " << path << " (" << width << "x" << height << ", " << channels << " channels)\n";

    // Prevent alignment issues
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Choose correct format based on actual channels
    GLenum format = GL_RGBA;
    GLenum internal = GL_RGBA8;

    if (channels == 3)
    {
        format = GL_RGB;
        internal = GL_RGB8;
    }
    else if (channels == 4)
    {
        format = GL_RGBA;
        internal = GL_RGBA8;
    }
    else if (channels == 1)
    {
        format = GL_RED;
        internal = GL_R8;
    }
    else
    {
        std::cerr << "Unsupported channel count: " << channels << "\n";
        stbi_image_free(data);
        return UINT32_MAX;
    }

    TextureDesc desc;
    desc.internalFormat = internal;
    desc.format = format;

    auto tex = std::make_unique<Texture>(width, height, data, desc);
    stbi_image_free(data);

    // Use vector size as ID
    TextureID id = static_cast<TextureID>(textures.size());
    textures.push_back({ std::move(tex), path });
    lookup[key] = id;

    std::cout << "Texture loaded successfully (ID: " << id << ")\n";
    return id;
}

Texture* TextureManager::getTexture(TextureID id) {
    if (id >= textures.size()) {
        std::cerr << "Invalid texture ID: " << id << "\n";
        return nullptr;
    }
    return textures[id].texture.get();
}

TextureID TextureManager::getID(const std::string& path) const {
    TextureKey key{ path };
    if (auto it = lookup.find(key); it != lookup.end()) {
        return it->second;
    }
    return UINT32_MAX;
}

void TextureManager::NextTexture(EventBus* bus) {

}
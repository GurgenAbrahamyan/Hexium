#pragma once
#include "TextureManager.h"
#include "../stb/stb_image.h"
#include <iostream>

TextureManager::TextureManager() {};

Texture* TextureManager::getTexture(const std::string path, TextureUsage usage)
{
    if (auto it = textures.find(path); it != textures.end()) {

		std::cout << "Texture already loaded: " << path << "\n";
        return it->second.get();

    }
    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << path << "\n";
        return nullptr;
    }

    TextureDesc desc;

    switch (usage)
    {
      
    case TextureUsage::Color:
        desc.format = (channels == 4) ? GL_RGBA : GL_RGB;
        desc.internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;
        break;

    case TextureUsage::Specular:
        std::cout << "Specular or Data texture loaded as single channel: " << path << "\n";
        desc.format = GL_RED;
        desc.internalFormat = GL_R8;

        break;
    case TextureUsage::Data:
        // force single channel

		std::cout << "Specular or Data texture loaded as single channel: " << path << "\n";
        desc.format = GL_RED;
        desc.internalFormat = GL_R8;
     
        break;

    case TextureUsage::Normal:
        desc.format = GL_RGB;
        desc.internalFormat = GL_RGB8;
        break;
    }

    auto tex = std::make_unique<Texture>(w, h, data, desc);
   
    stbi_image_free(data);

    Texture* raw = tex.get();
    textures[path] = std::move(tex);
    return raw;
}



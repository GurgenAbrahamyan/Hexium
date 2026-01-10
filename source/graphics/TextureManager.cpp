#pragma once
#include "TextureManager.h"
#include "../stb/stb_image.h"
#include <iostream>

TextureManager::TextureManager() {};

Texture* TextureManager::getTexture(std::string path) {
    if (auto it = textures.find(path); it != textures.end()) {
        std::cout << "Texture found: " << path << "\n";
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
    
    desc.format = (channels == 4) ? GL_RGBA : GL_RGB;
    
    desc.internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;

    
    auto tex = std::make_unique<Texture>(w, h, data, desc);
    
    stbi_image_free(data);

    Texture* raw = tex.get();
    textures[path] = std::move(tex);
    return raw;
}


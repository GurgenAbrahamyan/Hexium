#include "TextureManager.h"
#include "../stb/stb_image.h"
#include <iostream>
#include "../../core/Event.h"
#include "../../core/EventBus.h"

TextureID TextureManager::addTexture(const std::string& path,const TextureType& type)
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
        internal = isSRGB(type) ? GL_SRGB8 : GL_RGB8;
    }
    else if (channels == 4)
    {
        format = GL_RGBA;
        internal = isSRGB(type) ? GL_SRGB8_ALPHA8 : GL_RGBA8;
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


    TextureID id = static_cast<TextureID>(textures.size());
    textures.push_back({ std::move(tex), path, type });
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

bool TextureManager::isSRGB(TextureType type) const {

    switch (type) {
    case TextureType::Albedo:
        return true;
    default:
        return false;
    }
}

CubeMap* TextureManager::loadCubeMap(std::string filepath) {
    // TO-DO: fix 
    stbi_set_flip_vertically_on_load(false);

    int w, h, ch;
    unsigned char* data = stbi_load(filepath.c_str(), &w, &h, &ch, 0);
    if (!data) {
        std::cerr << "Failed to load cubemap\n";
        return nullptr;
    }

    int faceSize = w / 4;
    GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;

    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    struct Face { GLenum target; int gx, gy; };
    Face faceMap[6] = {
        { GL_TEXTURE_CUBE_MAP_POSITIVE_X, 2, 1 },
        { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 1 },
        { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 1, 0 },
        { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 1, 2 },
        { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 1, 1 },
        { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 3, 1 },
    };

    // Extract each face into contiguous memory
    std::vector<unsigned char> faceData(faceSize * faceSize * ch);

    for (auto& f : faceMap) {
        // Copy row by row
        for (int y = 0; y < faceSize; y++) {
            unsigned char* srcRow = data + ((f.gy * faceSize + y) * w + f.gx * faceSize) * ch;
            unsigned char* dstRow = faceData.data() + y * faceSize * ch;
            memcpy(dstRow, srcRow, faceSize * ch);
        }

        glTexImage2D(f.target, 0, format, faceSize, faceSize, 0, format, GL_UNSIGNED_BYTE, faceData.data());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    stbi_image_free(data);

    cubeMap = std::make_unique<CubeMap>();
    cubeMap->setTexture(texID);

    return cubeMap.get();
}
CubeMap* TextureManager::loadCubeMapArray(std::vector<std::string> filepaths) {
    if (filepaths.size() != 6) {
        std::cerr << "CubeMap requires exactly 6 faces, got " << filepaths.size() << std::endl;
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(false);

    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    for (unsigned int i = 0; i < 6; i++) {
        int width, height, channels;
        unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &channels, 0);

        if (!data) {
            std::cerr << "Failed to load cubemap face: " << filepaths[i] << std::endl;
            glDeleteTextures(1, &texID);
            return nullptr;
        }

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8; // linear

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            internalFormat,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );

        stbi_image_free(data);

        std::cout << "Loaded cubemap face " << i << ": " << filepaths[i] << std::endl;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    cubeMap = std::make_unique<CubeMap>();
    cubeMap->setTexture(texID);

    return cubeMap.get();
}


CubeMap* TextureManager::loadCubeMapDebug() {
    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    // Solid colors: RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN
    unsigned char colors[6][3] = {
        {255, 0, 0},   // +X = RED
        {0, 255, 0},   // -X = GREEN
        {0, 0, 255},   // +Y = BLUE
        {255, 255, 0}, // -Y = YELLOW
        {255, 0, 255}, // +Z = MAGENTA
        {0, 255, 255}  // -Z = CYAN
    };

    for (int i = 0; i < 6; i++) {
        unsigned char face[3];
        memcpy(face, colors[i], 3);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, face);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    cubeMap = std::make_unique<CubeMap>();
    cubeMap->setTexture(texID);
    return cubeMap.get();
}

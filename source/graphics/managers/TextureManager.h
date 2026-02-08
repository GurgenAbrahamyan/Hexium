#pragma once
#include <../glad/glad.h>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "../resources/Texture.h"
#include "../resources/CubeMap.h"
using TextureID = uint32_t;

class EventBus;

struct TextureKey {
    std::string path;
    bool operator==(const TextureKey& other) const {
        return path == other.path;
    }
};

struct TextureKeyHash {
    std::size_t operator()(const TextureKey& key) const {
        return std::hash<std::string>()(key.path);
    }
};

struct TextureRecord {
    std::unique_ptr<Texture> texture;
    std::string path;
};

class TextureManager {
public:
    TextureManager() = default;

    TextureID addTexture(const std::string& path);
    Texture* getTexture(TextureID id);

    TextureID getID(const std::string& path) const;
    void NextTexture(EventBus* bus);

    size_t getTextureCount() const { return textures.size(); }

    CubeMap* loadCubeMap(std::string filePath);
    CubeMap* loadCubeMapArray(std::vector<std::string> filepaths);
    CubeMap* loadCubeMapDebug();

private:
    std::vector<TextureRecord> textures;
    std::unordered_map<TextureKey, TextureID, TextureKeyHash> lookup;

	std::unique_ptr<CubeMap> cubeMap;
};
#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

class Material;
class EventBus;
class TextureManager;

// Define standard texture slot indices
enum class TextureSlot {
    BASE_COLOR = 0,
    NORMAL_MAP = 1,
    METALLIC_ROUGHNESS = 2,
    OCCLUSION = 3,
    EMISSIVE = 4,
    MAX_SLOTS = 5
};

class MaterialManager {
public:
    MaterialManager(TextureManager* texMgr, EventBus* bus);

    int addMaterial(const std::string& name,
        const std::vector<std::string>& texturePaths,
        float metallic = 0.0f,
        float roughness = 1.0f,
        float ao = 1.0f);

    Material* getMaterial(int id);
    int getMaterialID(const std::string& name);

private:
    TextureManager* textureManager;
    EventBus* bus;
    std::unordered_map<int, std::unique_ptr<Material>> idMap;
    std::unordered_map<std::string, int> nameToIDMap;
    int nextID = 0;


};
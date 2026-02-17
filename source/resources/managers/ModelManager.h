#pragma once
#include <unordered_map>
#include <string>
#include "../../include/EnTT/entt.hpp"
#include "../../math_custom/Vector3.h"
#include "../../math_custom/Quat.h"
#include "../../resources/data/ModelLoadConfig.h" 
class EventBus;
class MeshManager;
class MaterialManager;
class TextureManager;

class ModelManager {
public:
    ModelManager(
        EventBus* bus,
        MeshManager* meshManager,
        MaterialManager* materialManager,
        TextureManager* textureManager
    );

   
    entt::entity loadModel(
        const std::string& name,
        const std::string& path,
        entt::registry&    registry,
        const ModelLoadConfig& config
    );

    
    void destroyModel(entt::entity root, entt::registry& registry);

    entt::entity getModel(const std::string& name) const;

    bool isLoaded(const std::string& name) const;

private:
    EventBus* bus;
    MeshManager* meshManager;
    MaterialManager* materialManager;
    TextureManager* textureManager;

    std::unordered_map<std::string, entt::entity> loadedModels;
};
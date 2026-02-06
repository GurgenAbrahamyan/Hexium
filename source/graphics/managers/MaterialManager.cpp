#include "MaterialManager.h"
#include "TextureManager.h"
#include "../resources/Material.h"
#include "../data/MaterialData.h"
#include "../../core/EventBus.h"
#include "../../core/Event.h"



#include <iostream>

MaterialManager::MaterialManager(TextureManager* texMgr, EventBus* bus)
    : textureManager(texMgr), bus(bus) {

    bus->subscribe<InitMaterial>([this](InitMaterial& event) {
        const MaterialData* matData = event.data;
        this->addMaterial(
            matData->name,
            matData->texturePaths,
            matData->metallic,
            matData->roughness,
            matData->ao
        );
		event.result = this->getMaterial(this->getMaterialID(matData->name));
		});

}

int MaterialManager::addMaterial(const std::string& name,
    const std::vector<std::string>& texturePaths,
    float metallic,
    float roughness,
    float ao)
{
    // Check if material already exists
    if (auto it = nameToIDMap.find(name); it != nameToIDMap.end()) {
        std::cout << "Material already exists: " << name << " (ID: " << it->second << ")\n";
        return it->second;
    }

    int id = nextID++;
    auto mat = std::make_unique<Material>();

    std::cout << "Creating material: " << name << " (ID: " << id << ")\n";
    std::cout << "  Texture paths provided: " << texturePaths.size() << "\n";



    for (size_t i = 0; i < texturePaths.size() && i < static_cast<size_t>(TextureSlot::MAX_SLOTS); ++i) {
        const std::string& texPath = texturePaths[i];

        if (texPath.empty()) {
            std::cout << "  Slot " << i << ": EMPTY (skipped)\n";
            continue;
        }

        // Load texture and get Texture pointer
        uint32_t texID = textureManager->addTexture(texPath);
        if (texID == UINT32_MAX) {
            std::cerr << "  Slot " << i << ": FAILED to load " << texPath << "\n";
            continue;
        }

        Texture* tex = textureManager->getTexture(texID);
        if (!tex) {
            std::cerr << "  Slot " << i << ": Retrieved null texture for ID " << texID << "\n";
            continue;
        }

        mat->SetTexture(i, tex);

        // Print what we're assigning
        std::string slotName = "UNKNOWN";
        switch (i) {
        case 0: slotName = "BASE_COLOR"; break;
        case 1: slotName = "METALLIC_ROUGHNESS"; break;
        case 2: slotName = "NORMAL_MAP"; break;
        case 3: slotName = "OCCLUSION"; break;
        case 4: slotName = "EMISSIVE"; break;
        }

        std::cout << "  Slot " << i << " (" << slotName << "): " << texPath << " [TexID: " << texID << "]\n";
    }


    mat->metallic = metallic;
    mat->roughness = roughness;
    mat->ao = ao;
    mat->setID(id);

    std::cout << "  Properties: Metallic=" << metallic << ", Roughness=" << roughness << ", AO=" << ao << "\n";

    idMap[id] = std::move(mat);
    nameToIDMap[name] = id;

    std::cout << "Material created successfully!\n";
    return id;
}

Material* MaterialManager::getMaterial(int id) {
    if (auto it = idMap.find(id); it != idMap.end())
        return it->second.get();

    std::cerr << "Material not found with ID: " << id << "\n";
    return nullptr;
}

int MaterialManager::getMaterialID(const std::string& name) {
    if (auto it = nameToIDMap.find(name); it != nameToIDMap.end())
        return it->second;

    std::cerr << "Material not found with name: " << name << "\n";
    return -1;
}
#include "MaterialManager.h"
#include "TextureManager.h"
#include "../Material.h"
#include <iostream>

MaterialManager::MaterialManager(TextureManager* texMgr) {
    textureManager = texMgr;
}

int MaterialManager::addMaterial(const std::string& name,
    const std::vector<std::string>& textureIDs,
    float metallic,
    float roughness,
    float ao)
{
 
    if (auto it = nameToIDMap.find(name); it != nameToIDMap.end())
        return it->second;

    int id = nextID++;
    auto mat = std::make_unique<Material>();

   
    for (int i = 0; i < textureIDs.size(); ++i) {
        Texture* tex = textureManager->getTexture(textureManager->addTexture(textureIDs[i]));
        
        mat->SetTexture(i, tex);
    }

    mat->metallic = metallic;
    mat->roughness = roughness;
    mat->ao = ao;
	mat->setID(id);

    idMap[id] = std::move(mat);
    nameToIDMap[name] = id;

    std::cout << "Created material: " << name << " (ID: " << id << ")\n";
    std::cout << "texture name";
    return id;
}

Material* MaterialManager::getMaterial(int id) {
    if (auto it = idMap.find(id); it != idMap.end())
        return it->second.get();
    return nullptr;
}

int MaterialManager::getMaterialID(const std::string& name) {
    if (auto it = nameToIDMap.find(name); it != nameToIDMap.end())
        return it->second;
    return -1; 
}

#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

class Material;
class TextureManager;

class MaterialManager {
public:
    MaterialManager(TextureManager* texMgr);

   
    int addMaterial(const std::string& name,
        const std::vector<std::string>& texturePaths,
        float metallic = 0.0f,
        float roughness = 1.0f,
        float ao = 1.0f);

    Material* getMaterial(int id);          
    int getMaterialID(const std::string& name); 

private:
    TextureManager* textureManager;

    std::unordered_map<int, std::unique_ptr<Material>> idMap;  
    std::unordered_map<std::string, int> nameToIDMap;       

    int nextID = 0;  
};

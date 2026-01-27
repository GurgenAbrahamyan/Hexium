#pragma once

#include <unordered_map>
#include <memory>
#include <string>

class Model;
class TextureManager;
class MaterialManager;
class MeshManager;

class ModelManager {
public:
    ModelManager(
        TextureManager* texMgr,
        MaterialManager* matMgr,
        MeshManager* meshMgr
    );

    int addModel(const std::string& name, const std::string& modelPath);

    Model* getModel(int id);
    int getModelID(const std::string& name) const;

private:
    TextureManager* textureManager;
    MaterialManager* materialManager;
    MeshManager* meshManager;

    std::unordered_map<int, std::unique_ptr<Model>> idMap;
    std::unordered_map<std::string, int> nameToIDMap;

    int nextID = 0;
};

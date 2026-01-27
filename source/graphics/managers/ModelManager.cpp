#include "ModelManager.h"

#include "TextureManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"

#include "../../utils/ModelLoader.h"
#include "../Model.h"
#include "../../graphics/RenderMesh.h"
#include "../../graphics/Material.h"

ModelManager::ModelManager(
    TextureManager* texMgr,
    MaterialManager* matMgr,
    MeshManager* meshMgr
)
    : textureManager(texMgr),
    materialManager(matMgr),
    meshManager(meshMgr)
{
}

int ModelManager::addModel(const std::string& name, const std::string& modelPath)
{
    // already loaded
    auto it = nameToIDMap.find(name);
    if (it != nameToIDMap.end())
        return it->second;

    // TEMP loader (correct)
    ModelLoader loader(modelPath);
    const ModelData& data = loader.getModelData();

    auto model = std::make_unique<Model>();

    // Create materials first
    std::vector<Material*> materialRefs;
    materialRefs.reserve(data.materials.size());

    for (const auto& matData : data.materials)
    {
        int matID = materialManager->addMaterial(
            matData.name,
            matData.texturePaths,
            matData.metallic,
            matData.roughness,
            matData.ao
        );

        materialRefs.push_back(materialManager->getMaterial(matID));
    }

  
    for (const auto& node : data.nodes)
    {
        if (node.meshIndex < 0)
            continue;

        const MeshData& meshData = data.meshes[node.meshIndex];

        RenderMesh* mesh = meshManager->getMesh( meshManager->addMesh(meshData.name, meshData.vertices, meshData.indices, false));

        Material* material = nullptr;
        if (!materialRefs.empty())
            material = materialRefs[0]; 

        model->addSubMesh(mesh, material, node.position, node.rotation, node.scale);

    
    }

    int id = nextID++;

	model->setID(id);

    idMap[id] = std::move(model);
    nameToIDMap[name] = id;

    return id;
}

Model* ModelManager::getModel(int id)
{
    auto it = idMap.find(id);
    if (it == idMap.end())
        return nullptr;
    return it->second.get();
}

int ModelManager::getModelID(const std::string& name) const
{
    auto it = nameToIDMap.find(name);
    if (it == nameToIDMap.end())
        return -1;
    return it->second;
}

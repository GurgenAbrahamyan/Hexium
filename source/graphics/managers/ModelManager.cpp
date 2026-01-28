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
    // Check if already loaded
    auto it = nameToIDMap.find(name);
    if (it != nameToIDMap.end()) {
        std::cout << "Model already loaded: " << name << " (ID: " << it->second << ")\n";
        return it->second;
    }

    std::cout << "Loading model: " << name << " from " << modelPath << "\n";

   
    ModelLoader loader(modelPath);
    const ModelData& data = loader.getModelData();
    auto model = std::make_unique<Model>();

    std::vector<Material*> materialRefs;
    materialRefs.reserve(data.materials.size());

    std::cout << "Creating " << data.materials.size() << " materials...\n";
    for (const auto& matData : data.materials)
    {
        int matID = materialManager->addMaterial(
            matData.name,
            matData.texturePaths,
            matData.metallic,
            matData.roughness,
            matData.ao
        );
        Material* mat = materialManager->getMaterial(matID);
        if (mat) {
            materialRefs.push_back(mat);
            std::cout << "  Material " << matData.name << " -> ID " << matID << "\n";
        }
        else {
            std::cerr << "  ERROR: Failed to get material " << matData.name << "\n";
            materialRefs.push_back(nullptr);
        }
    }


    std::cout << "Creating " << data.meshes.size() << " meshes...\n";
    for (const auto& node : data.nodes)
    {
        if (node.meshIndex < 0 || node.meshIndex >= static_cast<int>(data.meshes.size())) {
            std::cout << "  Skipping node (invalid mesh index)\n";
            continue;
        }

        const MeshData& meshData = data.meshes[node.meshIndex];

        
        RenderMesh* mesh = meshManager->getMesh(
            meshManager->addMesh(
                meshData.name,
                meshData.vertices,
                meshData.indices,
                false
            )
        );

        if (!mesh) {
            std::cerr << "  ERROR: Failed to create mesh " << meshData.name << "\n";
            continue;
        }

       
        Material* material = nullptr;
        if (meshData.materialIndex >= 0 && meshData.materialIndex < static_cast<int>(materialRefs.size())) {
            material = materialRefs[meshData.materialIndex];
            std::cout << "  Node: " << node.name << " -> Mesh: " << meshData.name
                << " -> Material: " << data.materials[meshData.materialIndex].name << "\n";
        }
        else {
            std::cerr << "  WARNING: Node " << node.name << " has invalid material index "
                << meshData.materialIndex << " (using first material or none)\n";
            if (!materialRefs.empty()) {
                material = materialRefs[0];
            }
        }

       
        model->addSubMesh(
            mesh,
            material,
            node.position,
            node.rotation,
            node.scale,
            node.worldTransform
        );
    }

  
    int id = nextID++;
    model->setID(id);
    idMap[id] = std::move(model);
    nameToIDMap[name] = id;

    std::cout << "Model loaded successfully! (ID: " << id << ")\n";
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

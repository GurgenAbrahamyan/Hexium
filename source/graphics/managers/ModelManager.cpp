#include "ModelManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "../../utils/ModelLoader.h"
#include "../resources/Model.h"
#include "../../graphics/resources/RenderMesh.h"
#include "../../graphics/resources/Material.h"
#include "../../core/Eventbus.h"
#include "../../core/Event.h"
#include "../data/MeshData.h"
#include "../data/MaterialData.h"

ModelManager::ModelManager(EventBus* bus)
    : bus(bus)
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
    int modelID = nextID++;
    model->setID(modelID);

    // Load all materials
    std::vector<Material*> materialRefs;
    materialRefs.reserve(data.materials.size());

    std::cout << "Creating " << data.materials.size() << " materials...\n";
    for (const auto& matData : data.materials)
    {
        InitMaterial initEvent(const_cast<MaterialData*>(&matData));
        bus->publish<InitMaterial>(initEvent);

        Material* mat = initEvent.result;
        if (mat) {
            materialRefs.push_back(mat);
            std::cout << "  Material created -> ID " << mat->getID() << "\n";
        }
        else {
            std::cerr << "  ERROR: Failed to create material\n";
            materialRefs.push_back(nullptr);
        }
    }

    // Load all meshes and create submeshes
    std::cout << "Creating " << data.submeshes.size() << " submeshes...\n";
    int submeshID = 0;

    for (const auto& submeshData : data.submeshes)
    {
        // Validate mesh index
        if (submeshData.meshIndex >= data.meshes.size()) {
            std::cerr << "  ERROR: Submesh has invalid mesh index " << submeshData.meshIndex << "\n";
            continue;
        }
        MeshData meshData = data.meshes[submeshData.meshIndex];
        meshData.name = name + "::" + submeshData.name;

        InitMesh meshDataEvent(&meshData);
        bus->publish<InitMesh>(meshDataEvent);
        RenderMesh* renderMesh = meshDataEvent.result;

        if (!renderMesh) {
            std::cerr << "  ERROR: Failed to create mesh " << meshData.name << "\n";
            continue;
        }

        // Get material (may be nullptr if not set)
        Material* material = nullptr;
        if (submeshData.materialIndex != UINT32_MAX && submeshData.materialIndex < materialRefs.size()) {
            material = materialRefs[submeshData.materialIndex];
        }

        // Add submesh to model with world transform
        model->addSubMesh(
            submeshID,
            renderMesh,
            material,
            Vector3(),
            Quat(),
            Vector3(),
            submeshData.worldTransform
        );

        std::cout << "  Submesh created -> ID " << submeshID
            << " (mesh: " << meshData.name
            << ", material: " << (material ? std::to_string(material->getID()) : "none") << ")\n";

        submeshID++;
    }

    // Store model
    idMap[modelID] = std::move(model);
    nameToIDMap[name] = modelID;

    std::cout << "Model loaded successfully! (ID: " << modelID << ")\n";
    return modelID;
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
#include "MeshManager.h"
#include "../resources/StaticMesh.h"
#include "../../core/Event.h"
#include "../../core/EventBus.h"
#include "../data/MeshData.h"
MeshManager::MeshManager(EventBus* bus)
    : bus(bus)
{
    bus->subscribe<InitMesh>([this](InitMesh& event) {

        std::cout << "Initializing mesh: " << event.data->name << "\n";
        MeshID id = this->addMesh(
			
            event.data->name,
            event.data->vertices,
            event.data->indices,
            true
        );
        event.result = getMesh(id);
		});
}
MeshID MeshManager::addMesh(
    const std::string& name,
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    bool dynamic)
{
    if (auto it = nameToID.find(name); it != nameToID.end())
        return it->second;

    MeshID id = nextID++;

    std::unique_ptr<RenderMesh> mesh;
    if (dynamic)
        mesh = std::make_unique<StaticMesh>(vertices, indices);
    else
        mesh = std::make_unique<StaticMesh>(vertices, indices); // StaticMesh later

	mesh->setID(id);

    meshes[id] = std::move(mesh);
    nameToID[name] = id;

    return id;
}

RenderMesh* MeshManager::getMesh(MeshID id) {
    if (auto it = meshes.find(id); it != meshes.end())
        return it->second.get();
    return nullptr;
}

MeshID MeshManager::getMeshID(const std::string& name) const {
    if (auto it = nameToID.find(name); it != nameToID.end())
        return it->second;
    return -1;
}

#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

#include "../RenderMesh.h"
#include "../Vertex.h"

using MeshID = int;

class MeshManager {
public:
    MeshManager() = default;

    MeshID addMesh(
        const std::string& name,
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        bool dynamic = false
    );

    RenderMesh* getMesh(MeshID id);
    MeshID getMeshID(const std::string& name) const;

    int getMeshCount() const { return static_cast<int>(meshes.size()); }

private:
    std::unordered_map<MeshID, std::unique_ptr<RenderMesh>> meshes;
    std::unordered_map<std::string, MeshID> nameToID;

    MeshID nextID = 0;
};

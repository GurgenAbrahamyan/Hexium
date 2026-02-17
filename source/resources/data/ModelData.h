#pragma once
#include <string>
#include <vector>
#include "MeshData.h"
#include "MaterialData.h"
#include "SubMeshData.h"

struct ModelData {
    std::vector<MeshData> meshes;
    std::vector<MaterialData> materials;
    std::vector<SubMeshData> submeshes;
    std::string modelName;
};
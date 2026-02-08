#pragma once
#include "../../graphics/resources/RenderMesh.h"
struct MeshBatch {
    RenderMesh* mesh;
    std::vector<Mat4> instances;

    MeshBatch() {
        instances.reserve(128); // Pre-allocate
    }
};

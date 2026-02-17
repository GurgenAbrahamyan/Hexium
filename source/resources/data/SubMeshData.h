#pragma once
#include <string>
#include <cstdint>
#include "../../math_custom/Vector3.h"
#include "../../math_custom/Quat.h"
#include "../../math_custom/Mat4.h"

struct SubMeshData {
    std::string name;         // submesh name
    uint32_t meshIndex = UINT32_MAX;     // which mesh asset to use
    uint32_t materialIndex = UINT32_MAX; // material asset
    uint32_t parentIndex = UINT32_MAX;   // index of parent submesh in array, if any
    uint32_t nodeIndex = UINT32_MAX;
    Vector3 position = Vector3(0.0f);
    Quat rotation = Quat();
    Vector3 scale = Vector3(1.0f);
    Mat4 worldTransform = Mat4();
};

#pragma once
#include <string.h>
#include <vector>
#include "../../math_custom/Vector3.h"
#include "../../math_custom/Quat.h"
#include "../../math_custom/Mat4.h"



#include <unordered_map>
struct SubMeshData
{

    Mat4 worldTransform;
    uint32_t meshIndex;

    uint32_t materialIndex;

    std::string name;

};
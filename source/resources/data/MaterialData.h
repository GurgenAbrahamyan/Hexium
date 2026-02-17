#pragma once
#include <string>
#include <vector>
#include "MaterialTextureInfo.h"
struct MaterialData {
    std::string name;
    std::vector<MaterialTextureInfo> textureInfo;
    float metallic = 0.0f;
    float roughness = 1.0f;
    float ao = 1.0f;
};

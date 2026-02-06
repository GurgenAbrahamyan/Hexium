#pragma once
#include <string>
#include <vector>

struct MaterialData {
    std::string name;
    std::vector<std::string> texturePaths;
    float metallic = 0.0f;
    float roughness = 1.0f;
    float ao = 1.0f;
};

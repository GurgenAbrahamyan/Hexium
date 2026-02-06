#pragma once
#include <string>
#include <vector>
#include "Vertex.h"

struct MeshData {
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

};

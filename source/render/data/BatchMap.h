#pragma once
#include <unordered_map>
#include "../../resources/assets/RenderMesh.h"
#include "../../resources/assets/Material.h"
#include "../data/ShaderType.h"
#include "../data/MeshBatch.h"
using MeshBatchMap = std::unordered_map<RenderMesh*, MeshBatch>;
using MaterialBatchMap = std::unordered_map<Material*, MeshBatchMap>;
using ShaderBatchMap = std::unordered_map<Shader*, MaterialBatchMap>;
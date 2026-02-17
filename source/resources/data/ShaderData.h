#pragma once
#include <string>
#include "../../render/backend/Shader.h"

struct ShaderData {
    std::string name;
    std::string vertexPath;
    std::string fragmentPath;
    std::string geometryPath;
    ShaderType type = ShaderType::UNKNOWN;
};

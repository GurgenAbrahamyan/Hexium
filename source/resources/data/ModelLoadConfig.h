#pragma once
#include "../../math_custom/Vector3.h"
#include "../../math_custom/Quat.h"

struct ModelLoadConfig {
    
    Vector3 position = { 0, 0, 0 };
    Quat    rotation = {};
    Vector3 scale = { 1, 1, 1 };

    
    bool    hasPhysics = false;
    float   mass = 1.0f;
    bool    isKinematic = false;
    bool    isStatic = false;
};
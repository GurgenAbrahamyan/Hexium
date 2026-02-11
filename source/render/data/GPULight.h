#pragma once
#include "../../math_custom/Vector3.h"
struct GPULight {
    int type;         // 4 bytes
    float intensity;  // 4 bytes
    float pad0;       // 4 bytes
    float pad1;       // 4 bytes

    Vector3 color;   // vec3
    float pad2;

    Vector3  position;
    float pad3;

    Vector3 direction;
    float pad4;

    float innerCone;
    float outerCone;
    float pad5[2];    // padding for std140 alignment
};

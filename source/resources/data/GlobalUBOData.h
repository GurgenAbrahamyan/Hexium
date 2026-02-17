#pragma once
#include "../../math_custom/Mat4.h"
#include "../../math_custom/Vector3.h"
struct GlobalUBOData {
    Mat4 view;
    Mat4 proj;
    Vector3 camPos;
    float time;
    Vector3 ambientLight;
    float padding; 
};
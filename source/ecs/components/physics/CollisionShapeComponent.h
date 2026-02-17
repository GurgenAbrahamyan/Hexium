#pragma once
#include <vector>
#include "../../../math_custom/Vector3.h"
#include "../../../math_custom/Quat.h"
struct CollisionShapeComponent {
    std::vector<Vector3> vertices;
    std::vector<unsigned int> indices;
    

    Vector3 localPosition = Vector3(0, 0, 0);
    Quat    localRotation = Quat();
    Vector3 localScale = Vector3(1, 1, 1);
};
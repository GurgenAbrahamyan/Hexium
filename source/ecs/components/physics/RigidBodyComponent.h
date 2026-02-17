#pragma once
#include "../../../math_custom/Vector3.h"

struct RigidBodyComponent {
    float mass =0 , invmass=0;
    Vector3 linearVelocity = Vector3(0, 0, 0);
    Vector3 angularVelocity = Vector3(0, 0, 0);
    Vector3 forceAccum = Vector3(0, 0, 0);
    Vector3 torqueAccum = Vector3(0, 0, 0);
    int orthoFrameCounter = 0;
};

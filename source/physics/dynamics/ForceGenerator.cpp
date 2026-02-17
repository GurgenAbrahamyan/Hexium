// ForceGenerator.cpp
#include "ForceGenerator.h"

void GravityGenerator::applyRigid(RigidBodyComponent& rb) {
    if (rb.invmass == 0.0f) return;
    rb.forceAccum = rb.forceAccum + g * rb.mass;  // reference, not pointer
}
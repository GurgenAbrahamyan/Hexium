
#pragma once
#include "../../ecs/components/core/TransformComponent.h"
#include "../../ecs/components/physics/RigidBodyComponent.h"

class RigidBodyIntegrator {
public:
    void integrate(RigidBodyComponent& rb, TransformComponent& transform, float dt);
};
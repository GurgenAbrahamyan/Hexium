
#pragma once
#include <vector>
#include <memory>
#include "../../include/EnTT/entt.hpp"
#include "../physics/ecs_systems/RigidBodyIntegrator.h"
#include "dynamics/ForceGenerator.h"


class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine() = default;

    void addForceGenerator(std::unique_ptr<ForceGenerator> gen);
    void update(entt::registry& registry, float dt);

private:
    void applyForces(entt::registry& registry);
    void integrateBodies(entt::registry& registry, float dt);

    std::vector<std::unique_ptr<ForceGenerator>> generators;
    RigidBodyIntegrator integrator;
};
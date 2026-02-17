
#include "PhysicsEngine.h"
#include "dynamics/ForceGenerator.h"
#include "../ecs/components/core/TransformComponent.h"
#include "../ecs/components/physics/RigidBodyComponent.h"

PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::addForceGenerator(std::unique_ptr<ForceGenerator> gen) {
    generators.push_back(std::move(gen));
}
void PhysicsEngine::update(entt::registry& registry, float dt) {
    applyForces(registry);
    integrateBodies(registry, dt);
}

// PhysicsEngine.cpp


void PhysicsEngine::applyForces(entt::registry& registry) {
    auto view = registry.view<RigidBodyComponent>();
    view.each([&](RigidBodyComponent& rb) {
        if (rb.invmass == 0.0f) return;
        for (auto& gen : generators)
            gen->applyRigid(rb);  // -> because unique_ptr
        });
}

void PhysicsEngine::integrateBodies(entt::registry& registry, float dt) {
    auto view = registry.view<RigidBodyComponent, TransformComponent>();
    view.each([&](RigidBodyComponent& rb, TransformComponent& transform) {
        integrator.integrate(rb, transform, dt);
        });
}
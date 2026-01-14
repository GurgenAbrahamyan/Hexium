#include "PhysicsEngine.h"
#include "../dynamics/GeneralIntegrators.h"
#include "../math_custom/Vector3.h"
#include "../dynamics/Particle.h"
#include <iostream>
#include "../dynamics/ForceGenerator.h"
#include "../shapes/Object3D.h"

PhysicsEngine::PhysicsEngine(std::vector<Object3D*>& objects)
    : object3DS(objects)
{
    Vector3 gravity(0, -9.81f, 0);
  //  generators.push_back(new GravityGenerator(gravity));
}

PhysicsEngine::~PhysicsEngine() {
    for (auto* gen : generators)
        delete gen;
}

void PhysicsEngine::update(float delta) {
    for (Object3D* obj : object3DS) {
       
        for (auto* gen : generators)
            obj->applyForce(gen/*, delta*/);

        obj->getPhysics()->integrate(delta);
    }
}

void PhysicsEngine::setObjects(const std::vector<Object3D*>& objects) {
    object3DS = objects;
}

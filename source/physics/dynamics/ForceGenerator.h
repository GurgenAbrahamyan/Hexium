#pragma once
#include <vector>
#include "../../ecs/components/physics/RigidBodyComponent.h"


class Vector3;

class ForceGenerator {
public:
    virtual ~ForceGenerator() = default;
    virtual void applyRigid(RigidBodyComponent& object) = 0;
	

};

class GravityGenerator : public ForceGenerator{
public:
    Vector3 g;
    GravityGenerator(Vector3& g) : g(g){}

    void applyRigid(RigidBodyComponent& object) override;
};



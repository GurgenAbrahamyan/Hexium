#pragma once
#include <vector>
#include "../math_custom/Vector3.h"


class RigidBody;

class ForceGenerator {
public:
    virtual ~ForceGenerator() = default;
    virtual void applyRigid(RigidBody* object, float dt) = 0;
	

};

class GravityGenerator : public ForceGenerator{
public:
    Vector3 g;
    GravityGenerator(Vector3& g) : g(g){}

    void applyRigid(RigidBody* object, float dt) override;
};



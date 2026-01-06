#pragma once

#include "../math_custom/Mat3.h"
#include "../math_custom/Vector3.h"
#include "Object3d.h"    
#include "../dynamics/ForceGenerator.h"

class RigidBody : public Object3D {
public:
    RigidBody(Vector3 pos = Vector3(0, 0, 0), float mass = 1.0f);
    virtual ~RigidBody() = default;

    void applyForce(ForceGenerator* gen, float dt);
    void addForceToAccum(const Vector3& force);
    void integrate(float dt) override;

    float getMass() const;
    Matrix3 getOrientation() const;

private:
    int orthoFrameCounter = 0;

public:
    float mass;
    float invMass;

    Vector3 velocity;
    Vector3 forceAccum;

    Matrix3 orientation;
    Vector3 angularVelocity;
};

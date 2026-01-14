#pragma once

#include "../math_custom/Mat3.h"
#include "../math_custom/Vector3.h"
#include "PhysicsObject.h"    
#include "../dynamics/ForceGenerator.h"

class RigidBody : public PhysicsObject {
public:
    RigidBody(Vector3 pos = Vector3(0, 0, 0), float mass = 1.0f, Vector3 scale = Vector3(1, 1, 1), const std::vector<float>& vertices = {}, const std::vector<unsigned int>& ind = {});
    virtual ~RigidBody() = default;

    void applyForce(ForceGenerator* gen) override;
    void addForceToAccum(const Vector3& force);
    void integrate(float dt) override;

    float getMass() const;
    Matrix3* getOrientation();

    
    Vector3 getScale(){ return scale; }
    void setScale(const Vector3& s) { scale = s; }
    Vector3 getAngularVelocity() override;
    void setAngularVelocity(const Vector3& angVel)override {
		angularVelocity = angVel;
    }

private:
    int orthoFrameCounter = 0;

public:
    float mass;
    float invMass;

    Vector3 velocity;
    Vector3 forceAccum;

    
    Vector3 angularVelocity;
    Vector3 scale;  
};

#pragma once
#include "../math_custom/Vector3.h"
#include "../math_custom/Mat3.h"
#include "../dynamics/Particle.h"
#include "../dynamics/ForceGenerator.h"
class PhysicsObject {
public:
 
    Vector3 position;
    Vector3 velocity;
    Matrix3 orientation;


    ParticleSystem* particles = nullptr;

    virtual void integrate(float dt) = 0;
    virtual void applyForce(ForceGenerator* gen) = 0;

    virtual Matrix3 getOrientation() const {
		return orientation;
	}

    virtual Vector3 getAngularVelocity() {
		return Vector3(0, 0, 0);
    }
    virtual void setAngularVelocity(const Vector3& angVel) {}
    virtual ~PhysicsObject() = default;
};
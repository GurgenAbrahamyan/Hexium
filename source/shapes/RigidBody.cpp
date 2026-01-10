#include "RigidBody.h"

RigidBody::RigidBody(Vector3 pos, float mass)
    : mass(mass), invMass(1 / mass), angularVelocity(Vector3(0, 0, 0.0f))
{
    position = pos;
    orientation.setIdentity();
}

void RigidBody::applyForce(ForceGenerator* gen, float dt) {
    gen->applyRigid(this, dt);
}

void RigidBody::addForceToAccum(const Vector3& force) {
    forceAccum = forceAccum.add(force);
}

void RigidBody::integrate(float dt) {
    Vector3 acc = forceAccum.multiply(invMass);
    velocity = velocity.add(acc.multiply(dt));
    position = position.add(velocity.multiply(dt));
    forceAccum = Vector3(0, 0, 0);

    float angVelLengthSq = angularVelocity.lengthSquared();
    if (angVelLengthSq > 0.000001f) {
        Matrix3 omegaHat;
        omegaHat = omegaHat.makeOmegaMatrix(angularVelocity);

        Matrix3 dR = omegaHat * orientation * dt;

        orientation = orientation + dR;

        orthoFrameCounter++;
        if (orthoFrameCounter % 20 == 0) {
            orientation.orthonormalize();
        }
    }
}

float RigidBody::getMass() const {
    return mass;
}

Matrix3 RigidBody::getOrientation() const {
    return orientation;
}

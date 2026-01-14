#include "RigidBody.h"





RigidBody::RigidBody(Vector3 pos, float mass, Vector3 scale, const std::vector<float>& vertices, const std::vector<unsigned int>& ind)
    : mass(mass), invMass(1 / mass), angularVelocity(Vector3(0, 0, 0.0f)), scale(scale)
{
    position = pos;
    orientation.setIdentity();

	particles = new ParticleSystem(mass);

    for (int x = 0; x < vertices.size(); x += 3) {
        Particle* p = new Particle(mass / vertices.size() / 3, Vector3(vertices[x], vertices[x + 1], vertices[x + 2]));
        particles->addParticle(p);
    }

    //indicies unused for now

}

void RigidBody::applyForce(ForceGenerator* gen) {
    gen->applyRigid(this, 0.0);
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

Matrix3* RigidBody::getOrientation() {
    return &orientation;
}

Vector3 RigidBody::getAngularVelocity() {
    return angularVelocity;
}





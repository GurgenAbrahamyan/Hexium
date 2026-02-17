
#include "RigidBodyIntegrator.h"
#include "../../math_custom/Quat.h"
void RigidBodyIntegrator::integrate(RigidBodyComponent& rb, TransformComponent& transform, float dt) {
    if (rb.invmass == 0.0f) return;

   
    Vector3 linearAcc = rb.forceAccum * rb.invmass;
    rb.linearVelocity = rb.linearVelocity + linearAcc * dt;
    transform.position = transform.position + rb.linearVelocity * dt;

   
    Vector3 angularAcc = rb.torqueAccum * rb.invmass;
    rb.angularVelocity = rb.angularVelocity + angularAcc * dt;

    if (rb.angularVelocity.lengthSquared() > 0.000001f) {
        Quat spin(
            rb.angularVelocity.x * dt,
            rb.angularVelocity.y * dt,
            rb.angularVelocity.z * dt,
            0.0f
        );
        transform.rotation = (transform.rotation + spin * transform.rotation * 0.5f).normalized();
    }

   
    rb.forceAccum = Vector3(0, 0, 0);
    rb.torqueAccum = Vector3(0, 0, 0);
}
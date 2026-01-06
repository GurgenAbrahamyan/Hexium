#include "../math_custom/Vector3.h"
#include "../shapes/RigidBody.h"


void GravityGenerator::applyRigid(RigidBody* object, float dt) {
    
       object->addForceToAccum(g.multiply(object->getMass()));
    
    }

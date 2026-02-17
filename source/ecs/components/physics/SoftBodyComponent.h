#pragma once
#include "../../../physics/particles/Particle.h"

struct SoftBodyComponent {
    ParticleSystem* particles;  // mass lives inside each particle
    float stiffness;
    float damping;
};
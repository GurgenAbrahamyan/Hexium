#pragma once

#include <vector>
#include "../shapes/Object3D.h"
#include "../dynamics/ForceGenerator.h"

class PhysicsEngine {
public:
    PhysicsEngine(std::vector<Object3D*>& objects);
    ~PhysicsEngine();

    void update(float delta);
    void setObjects(const std::vector<Object3D*>& objects);

private:
    std::vector<Object3D*>& object3DS;
    std::vector<ForceGenerator*> generators;
};

#pragma once

#include <vector>

class Object3D;
class ForceGenerator;


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

#pragma once

#include <vector>
#include "EventBus.h"
#include "Event.h"
#include "../shapes/Object3D.h"

class Scene {
public:
    Scene(EventBus* bus);
    ~Scene();

    std::vector<Object3D*>& objectList();

private:
    std::vector<Object3D*> objects;
};

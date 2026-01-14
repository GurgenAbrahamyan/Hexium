#pragma once

#include <vector>
#include "EventBus.h"
#include "Event.h"
#include "../shapes/Object3D.h"
#include "../graphics/Light.h"

class Scene {
public:
    Scene(EventBus* bus);
    ~Scene();

    std::vector<Object3D*>& objectList();
    std::vector<Light*>& lightsList();
    void initGpu();
private:
    std::vector<Object3D*> objects;
    std::vector<Light*> lights;
	TextureManager textureManager;
};

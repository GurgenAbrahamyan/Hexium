#include "Scene.h"
#include "../shapes/Rectangle.h"
#include "../graphics/RenderMesh.h"
#include "../math_custom/Vector3.h"
#include <iostream>

Scene::Scene(EventBus* bus) {
    bus->subscribe<CreateObject>([this](CreateObject& event) {
    
            Rectangle* rec = new Rectangle();
            std::cout << "Creating object\n" << objects.size();
            RenderMesh* mesh = rec->getMesh();
            mesh->setupBuffers();
            mesh->bind();

            objects.push_back(rec);
        
        });

  /*  objects.push_back(new Rectangle(Vector3(0, 0, 0), Vector3(10, 10, 10)));
    objects.push_back(new Rectangle(Vector3(0, 0, 10), Vector3(10, 10, 10)));*/
}

Scene::~Scene() {
    for (auto* obj : objects)
        delete obj;
}

std::vector<Object3D*>& Scene::objectList() {
    return objects;
}

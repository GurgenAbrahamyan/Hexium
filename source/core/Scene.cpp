#include "Scene.h"
#include "../shapes/Rectangle.h"
#include "../graphics/RenderMesh.h"
#include "../math_custom/Vector3.h"
#include <iostream>

Scene::Scene(EventBus* bus) {
	textureManager = TextureManager();


    bus->subscribe<CreateObject>([this](CreateObject& event) {
    
     
            Rectangle* rec = new Rectangle();
            std::cout << "Creating object\n" << objects.size();
        
            rec->initializeGPU(&textureManager);
            rec->createCrate(&textureManager);


            objects.push_back(rec);
        
        });

  
   // objects.push_back(new Rectangle(Vector3(0, 0, 0), Vector3(10, 10, 10)));
    objects.push_back(new Rectangle(Vector3(0, 0, 10), Vector3(10, 30, 50)));
    Rectangle* background = new Rectangle(Vector3(0, 0, 0), Vector3(1000, 1000, 1000));

   
    background->angularVelocity = Vector3(0, 0, 0);
        
    objects.push_back(background);
}

Scene::~Scene() {
    for (auto* obj : objects)
        delete obj;
}

std::vector<Object3D*>& Scene::objectList() {
    return objects;
}

void Scene::initGpu() {
    for(Object3D* obj : objects) {
        obj->initializeGPU(&textureManager);
       
	}

}

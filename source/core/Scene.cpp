#include "Scene.h"
#include "../shapes/Rectangle.h"
#include "../graphics/RenderMesh.h"
#include "../math_custom/Vector3.h"
#include <iostream>

Scene::Scene(EventBus* bus) {
	textureManager = TextureManager();


    bus->subscribe<CreateObject>([this](CreateObject& event) {
         for (int i = 0; i < 1000; i++) {
            Rectangle* rec = new Rectangle();
            std::cout << "Creating object\n" << objects.size();

            rec->initializeGPU(&textureManager);
        


            objects.push_back(rec);
        }

       
        });

  
  
    Rectangle* saqos = new Rectangle(Vector3(0, 0, 0), Vector3(10, 30, 30));
    objects.push_back(saqos);


    Rectangle* background = new Rectangle(Vector3(0, 0, 0), Vector3(250, 250, 250));
  
        Light* light1 = new Light(Vector3(1.0f, 1.0f, 1.0), Vector3(0.6,0.6, 0.6), 1.0f, LightType::Point);
		light1->setOwner(saqos);
       
        lights.push_back(light1);
  

	
        background->getPhysics()->setAngularVelocity(Vector3(0, 0, 0));;

   
		background->getPhysics()->position = Vector3(0, 0, 0);
   

    objects.push_back(background);
	
}

Scene::~Scene() {
    for (auto* obj : objects)
        delete obj;
}

std::vector<Object3D*>& Scene::objectList() {
    return objects;
}
std::vector<Light*>& Scene::lightsList() {
    return lights;
}

void Scene::initGpu() {
    for(Object3D* obj : objects) {
        obj->initializeGPU(&textureManager);
       
	}

}

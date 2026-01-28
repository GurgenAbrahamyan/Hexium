
#include<filesystem>
namespace fs = std::filesystem;

#include "Scene.h"

#include "EventBus.h"
#include "Event.h"

#include "../shapes/Object3D.h"
#include "../shapes/Rectangle.h"

#include "../graphics/Light.h"
#include "../graphics/RenderMesh.h"

#include "../graphics/managers/TextureManager.h"

#include "../math_custom/Vector3.h"
#include <iostream>


#include "../graphics/managers/MaterialManager.h"
#include "../graphics/managers/MeshManager.h"
#include "../graphics/managers/ModelManager.h"
#include "../utils/ModelLoader.h"



Scene::Scene(EventBus* bus){

	textureManager = new TextureManager();
	meshManager = new MeshManager();
    materialManager = new MaterialManager(textureManager);
	modelManager = new ModelManager(textureManager, materialManager, meshManager);




    bus->subscribe<CreateObject>([this](CreateObject& event) {
         for (int i = 0; i < 1000; i++) {
            Rectangle* rec = new Rectangle();
            std::cout << "Creating object\n" << objects.size();

            rec->initializeGPU(textureManager);
        


            objects.push_back(rec);
        }

       
        });
    bus->subscribe<PressedKey>([this](PressedKey& event) {
        if (event.key == 'Q') {
            // textureManager->nextTexture();

        }
        
        })
        ;



    
        }

        Scene::~Scene() {
            for (auto* obj : objects);
       // delete obj;
}

std::vector<Object3D*>& Scene::objectList() {
    return objects;
}
std::vector<Light*>& Scene::lightsList() {
    return lights;
}

void Scene::initObjects() {
   
          
          std::string modelPath = "resource/models/chess/chess_set_4k.gltf";
          
          Object3D* obj = new Object3D(nullptr, modelManager->getModel(modelManager->addModel("chess", modelPath)));

          obj->setPosition(Vector3(5, 5, 5));
          obj->getPhysics()->setAngularVelocity(Vector3(0, 0, 2));
          obj->setScale(Vector3(3, 3, 3));

          objects.push_back(obj);

        modelPath = "resource/models/map/scene.gltf";
           obj = new Object3D(nullptr, modelManager->getModel(modelManager->addModel("map", modelPath)));

          obj->setPosition(Vector3(10, 5, 5));
          obj->getPhysics()->setAngularVelocity(Vector3(0, 0, 2));
          obj->setScale(Vector3(0.2, 0.2, 0.2));

          objects.push_back(obj);
        modelPath = "resource/models/duck/Duck.gltf";
           obj = new Object3D(nullptr, modelManager->getModel(modelManager->addModel("duck", modelPath)));

          obj->setPosition(Vector3(15, 5, 5));
          obj->getPhysics()->setAngularVelocity(Vector3(0, 0, 2));
          obj->setScale(Vector3(1, 1, 1));

          objects.push_back(obj);
       modelPath = "resource/models/helmet/DamagedHelmet.gltf"; 
          obj = new Object3D(nullptr, modelManager->getModel(modelManager->addModel("helmet", modelPath)));

          obj->setPosition(Vector3(20, 5, 5));
          obj->getPhysics()->setAngularVelocity(Vector3(0, 0, 2));
          obj->setScale(Vector3(1, 1, 1));

          objects.push_back(obj);
     
            modelPath = "resource/models/toy car/ToyCar.gltf";

            obj = new Object3D(nullptr, modelManager->getModel(modelManager->addModel("car", modelPath)));

            obj->setPosition(Vector3(25, 5, 5));
            obj->setScale(Vector3(100, 100, 100));
            obj->getPhysics()->setAngularVelocity(Vector3(0, 0, 2));

            objects.push_back(obj);
    
          

      objects.push_back(new Rectangle(Vector3(3, 0, 0), Vector3(1, 1, 1)));
      objects.push_back(new Rectangle(Vector3(0, 3, 0), Vector3(1, 1, 1)));
      objects.push_back(new Rectangle(Vector3(0, 0, 3), Vector3(1, 1, 1)));

      objects.push_back(new Rectangle(Vector3(10, 0, 0), Vector3(1, 1, 1)));

   


    Rectangle* background = new Rectangle(Vector3(0, 0, 0), Vector3(500, 500, 500));

    Light* light1 = new Light(Vector3(1, 1, 1), Vector3(1,1, 1), 1.0f, LightType::Point);
    
	light1->setOwner(objects.at(0));

    lights.push_back(light1);



    background->getPhysics()->setAngularVelocity(Vector3(0, 0, 0));;


    background->getPhysics()->position = Vector3(0, 0, 0);


    objects.push_back(background);



    for (Object3D* obj : objects) {
        
        obj->initializeGPU(textureManager);
       
    }
}


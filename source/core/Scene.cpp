
#include<filesystem>
namespace fs = std::filesystem;

#include "Scene.h"

#include "EventBus.h"
#include "Event.h"

#include "../shapes/Object3D.h"
#include "../shapes/Rectangle.h"

#include "../graphics/resources/Light.h"
#include "../graphics/resources/RenderMesh.h"

#include "../graphics/managers/TextureManager.h"

#include "../math_custom/Vector3.h"
#include <iostream>


#include "../graphics/managers/MaterialManager.h"
#include "../graphics/managers/MeshManager.h"
#include "../graphics/managers/ModelManager.h"
#include "../utils/ModelLoader.h"



Scene::Scene(EventBus* bus){

    textureManager = new TextureManager();
    materialManager = new MaterialManager(textureManager, bus);
    meshManager = new MeshManager(bus);
	modelManager = new ModelManager(bus);


    Rectangle::InitializeSharedResources();

   /* bus->subscribe<CreateObject>([this](CreateObject& event) {
        for (int i = 0; i < 1000; i++) {
            float x = (rand() % 100) - 50;
            float z = (rand() % 100) - 50;
            std::cout << "Creating object\n" << objects.size();

            Rectangle* rec = new Rectangle(Vector3(x, 0, z));
            rec->getPhysics()->setAngularVelocity(Vector3(0, 0, 2));
            objects.push_back(rec);
        }

       
        });
        */
    bus->subscribe<CreateObject>([this](CreateObject& event) {
        for (int i = 0; i < 1000; i++) {
            float x = (rand() % 100) - 50;
            float z = (rand() % 100) - 50;
            std::cout << "Creating object\n" << objects.size();
          
            std::string modelPath = "resource/models/duck/Duck.gltf";
            Object3D* obj = new Object3D(nullptr, modelManager->getModel(modelManager->addModel("duck", modelPath)));

            obj->setPosition(Vector3(x, 0, z));
            obj->getPhysics()->setAngularVelocity(Vector3(0, 0, 2));
            obj->setScale(Vector3(1, 1, 1));
            objects.push_back(obj);
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

           modelPath = "resource/models/boombox_4k/boombox_4k.gltf";

            obj = new Object3D(nullptr, modelManager->getModel(modelManager->addModel("boombox", modelPath)));

            obj->setPosition(Vector3(30, 5, 5));
            obj->getPhysics()->setAngularVelocity(Vector3(0, 0, 2));
            obj->setScale(Vector3(3, 3, 3));

            objects.push_back(obj);

            modelPath = "resource/models/cannon_4k.gltf/cannon_01_4k.gltf";

            obj = new Object3D(nullptr, modelManager->getModel(modelManager->addModel("cannon", modelPath)));

            obj->setPosition(Vector3(35, 5, 5));
            obj->getPhysics()->setAngularVelocity(Vector3(0, 0, 2));
            obj->setScale(Vector3(3, 3, 3));

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


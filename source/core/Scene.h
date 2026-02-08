#pragma once


#include <vector>


class EventBus;
class Object3D;
class Light;
class ModelManager;
class TextureManager;
class MaterialManager;
class MeshManager;
class CubeMap;

class Scene {
public:
    Scene(EventBus* bus);
    ~Scene();
	
    std::vector<Object3D*>& objectList();
    std::vector<Light*>& lightsList();
	CubeMap* getSkybox() const { return skybox; }
    void initObjects();
private:
    std::vector<Object3D*> objects;
    std::vector<Light*> lights;
	CubeMap* skybox;

	TextureManager* textureManager;
    MaterialManager* materialManager;
    MeshManager* meshManager;
    ModelManager* modelManager;


	
    
    
};

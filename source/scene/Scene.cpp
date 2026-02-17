#include "Scene.h"
#include "../core/EventBus.h"
#include "../core/Event.h"

#include "../resources/managers/TextureManager.h"
#include "../resources/managers/MaterialManager.h"
#include "../resources/managers/MeshManager.h"
#include "../resources/managers/ModelManager.h"

#include "../ecs/factories/GraphicsEntityFactory.h"
#include "../ecs/factories/PhysicsComponentFactory.h"

#include "../ecs/components/graphics/LightComponent.h"
#include "../ecs/components/physics/RigidBodyComponent.h"
#include "../ecs/components/physics/CollisionShapeComponent.h"

#include "../core/EngineContext.h"
#include "../resources/data/ModelLoadConfig.h" 
Scene::Scene(EventBus* bus) {
    textureManager = new TextureManager();
    materialManager = new MaterialManager(textureManager, bus);
    meshManager = new MeshManager(bus);
    modelManager = new ModelManager(bus, meshManager, materialManager, textureManager);
}

Scene::~Scene() {
    // registry destructor cleans up all entities
    // managers own their GPU resources
    delete modelManager;
    delete meshManager;
    delete materialManager;
    delete textureManager;
}

void Scene::initObjects() {
    // ?? Skybox ????????????????????????????????????????????????????????????
    std::vector<std::string> faces = {
        "resource/textures/skybox/right.jpg",
        "resource/textures/skybox/left.jpg",
        "resource/textures/skybox/top.jpg",
        "resource/textures/skybox/bottom.jpg",
        "resource/textures/skybox/front.jpg",
        "resource/textures/skybox/back.jpg",
    };
    skybox = textureManager->loadCubeMapArray(faces);
    GraphicsEntityFactory::createSkybox(registry, skybox);

    GraphicsEntityFactory::createCamera(registry, Vector3(0, 0, 0), EngineContext::get().fov,(float) EngineContext::get().windowWidth/ EngineContext::get().windowHeight, EngineContext::get().nearPlane, EngineContext::get().farPlane, true, "camera");
        
    // ?? Load models ???????????????????????????????????????????????????????????
    auto chess = modelManager->loadModel("chess", "resource/models/chess/chess_set_4k.gltf", registry, { Vector3(5,5,5), Quat(), Vector3(3, 3, 3) });
    auto map = modelManager->loadModel("map", "resource/models/map/scene.gltf", registry, { Vector3(10,5,5), Quat(), Vector3(0.2,0.2,0.2) });
    auto duck = modelManager->loadModel("duck", "resource/models/duck/Duck.gltf", registry, { Vector3(15,5,5), Quat(), Vector3(1,1,1) });
    auto helmet = modelManager->loadModel("helmet", "resource/models/helmet/DamagedHelmet.gltf", registry, { Vector3(20,5,5), Quat(), Vector3(1,1,1) });
    auto car = modelManager->loadModel("car", "resource/models/toy car/ToyCar.gltf", registry, { Vector3(25,5,5), Quat(), Vector3(100,100,100) });
    auto boombox = modelManager->loadModel("boombox", "resource/models/boombox_4k/boombox_4k.gltf", registry, { Vector3(30,5,5), Quat(), Vector3(3,3,3) });
    auto cannon = modelManager->loadModel("cannon", "resource/models/cannon_4k.gltf/cannon_01_4k.gltf", registry, { Vector3(35,5,5), Quat(), Vector3(3,3,3) });

    // ?? Create rigid bodies using model transform ??????????????????????????????
    auto createRBWithModelTransform = [&](entt::entity e, float mass, const Vector3& pos, const Quat& rot, const Vector3& scale) {
        registry.emplace<RigidBodyComponent>(e, PhysicsComponentFactory::createRigidBody(registry, e, pos, rot, scale, mass));
        };

    // Chess
    createRBWithModelTransform(chess, 1.0f, Vector3(5, 5, 5), Quat(), Vector3(3, 3, 3));

    // Map (static)
    createRBWithModelTransform(map, 0.0f, Vector3(10, 5, 5), Quat(), Vector3(0.2f, 0.2f, 0.2f));

    // Duck
    createRBWithModelTransform(duck, 1.0f, Vector3(15, 5, 5), Quat(), Vector3(1, 1, 1));

    // Helmet
    createRBWithModelTransform(helmet, 1.0f, Vector3(20, 5, 5), Quat(), Vector3(1, 1, 1));

    // Car
    createRBWithModelTransform(car, 1.0f, Vector3(25, 5, 5), Quat(), Vector3(100, 100, 100));

    // Boombox
    createRBWithModelTransform(boombox, 1.0f, Vector3(30, 5, 5), Quat(), Vector3(3, 3, 3));

    // Cannon
    createRBWithModelTransform(cannon, 5.0f, Vector3(35, 5, 5), Quat(), Vector3(3, 3, 3));


    registry.emplace<CollisionShapeComponent>(chess, PhysicsComponentFactory::createCubeShape(Vector3(3, 3, 3)));
    registry.emplace<CollisionShapeComponent>(map, PhysicsComponentFactory::createCubeShape(Vector3(0.2, 0.2, 0.2)));
    registry.emplace<CollisionShapeComponent>(duck, PhysicsComponentFactory::createCubeShape(Vector3(1, 1, 1)));
    registry.emplace<CollisionShapeComponent>(helmet, PhysicsComponentFactory::createCubeShape(Vector3(1, 1, 1)));
    registry.emplace<CollisionShapeComponent>(car, PhysicsComponentFactory::createCubeShape(Vector3(100, 100, 100)));
    registry.emplace<CollisionShapeComponent>(boombox, PhysicsComponentFactory::createCubeShape(Vector3(3, 3, 3)));
    registry.emplace<CollisionShapeComponent>(cannon, PhysicsComponentFactory::createCubeShape(Vector3(3, 3, 3)));

    
    auto rect1 = GraphicsEntityFactory::createRectangle(registry, *meshManager, "rect1", Vector3(5, 0, 0), Vector3(3, 3, 3));
    auto rect2 = GraphicsEntityFactory::createRectangle(registry, *meshManager, "rect2", Vector3(0, 5, 0), Vector3(2, 2, 2));
    auto rect3 = GraphicsEntityFactory::createRectangle(registry, *meshManager, "rect3", Vector3(0, 0, 5), Vector3(1, 1, 1));
    auto rect4 = GraphicsEntityFactory::createRectangle(registry, *meshManager, "rect4", Vector3(10, 0, 0), Vector3(1, 1, 1));

    registry.emplace<RigidBodyComponent>(rect1, PhysicsComponentFactory::createStaticBody(registry, rect1));
    registry.emplace<RigidBodyComponent>(rect2, PhysicsComponentFactory::createStaticBody(registry, rect2));
    registry.emplace<RigidBodyComponent>(rect3, PhysicsComponentFactory::createStaticBody(registry, rect3));
    registry.emplace<RigidBodyComponent>(rect4, PhysicsComponentFactory::createStaticBody(registry, rect4));

    registry.emplace<CollisionShapeComponent>(rect1, PhysicsComponentFactory::createCubeShape(Vector3(3, 3, 3)));
    registry.emplace<CollisionShapeComponent>(rect2, PhysicsComponentFactory::createCubeShape(Vector3(2, 2, 2)));
    registry.emplace<CollisionShapeComponent>(rect3, PhysicsComponentFactory::createCubeShape(Vector3(1, 1, 1)));
    registry.emplace<CollisionShapeComponent>(rect4, PhysicsComponentFactory::createCubeShape(Vector3(1, 1, 1)));


    GraphicsEntityFactory::createLight(registry, LightType::Point, Vector3(0, 0, 1), 3.0f, Vector3(5, 5, 8));
    GraphicsEntityFactory::createLight(registry, LightType::Point, Vector3(1, 0.2, 0.2), 3.0f, Vector3(10, 8, 5));
    GraphicsEntityFactory::createLight(registry, LightType::Point, Vector3(0.2, 1, 0.2), 3.0f, Vector3(15, 8, 5));
    GraphicsEntityFactory::createLight(registry, LightType::Point, Vector3(0.2, 0.5, 1), 3.0f, Vector3(20, 8, 5));
    GraphicsEntityFactory::createLight(registry, LightType::Point, Vector3(1, 1, 0.2), 3.0f, Vector3(25, 8, 5));
    GraphicsEntityFactory::createLight(registry, LightType::Point, Vector3(1, 0.2, 1), 3.0f, Vector3(30, 8, 5));
    GraphicsEntityFactory::createLight(registry, LightType::Point, Vector3(0.2, 1, 1), 3.0f, Vector3(35, 8, 5));
}
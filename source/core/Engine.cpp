#include "Engine.h"


#include "EventBus.h"
#include "Scene.h"
#include "../render/Renderer.h"
#include "PhysicsEngine.h"
#include "../render/UiRender.h"
#include "../input/UIInput.h"
#include "../input/KeyboardInput.h"

#include "../render/camera.h"
#include "../input/MouseInput.h"
#include "EngineContext.h"


Engine::Engine()
    : bus(new EventBus()),
    scene(new Scene(bus)),
    camera(new Camera(bus)),
    renderer(new Renderer(bus)),
    physicsEngine(new PhysicsEngine(scene->objectList())),
    ui(new UiInput(bus)),
    uiRender(new UiRender()),
    keyboardInput(new KeyboardInput(bus)),
    mouseInput(new MouseInput(bus)),
    running(true),
    accumulator(0.0f),
    framecount(0),
    framesThisSecond(0),
    timeSinceLastFpsPrint(0.0f)
{
    window = EngineContext::get().getWindow();
    scene->initObjects();
	renderer->markBatchesDirty();
    

    
    
}

Engine::~Engine() {
    delete bus;
    delete scene;
    delete renderer;
    delete physicsEngine;
    delete ui;
    delete uiRender;
    delete keyboardInput;
    delete mouseInput;
}

void Engine::run() {
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = now - lastTime;
        lastTime = now;
        float frameTime = delta.count();


        accumulator += frameTime;


        while (accumulator >= PHYSICS_STEP) {
            
            physicsEngine->update(PHYSICS_STEP);
            accumulator -= PHYSICS_STEP;
        }

        EngineContext::get().deltaTime = frameTime;

        keyboardInput->processInput();
		
        mouseInput->proccessInput();
        ui->processInput();
        renderer->render(scene, camera);

        glfwPollEvents();

        framesThisSecond++;
        timeSinceLastFpsPrint += frameTime;

        if (timeSinceLastFpsPrint >= 1.0f) {
            std::cout << "FPS: " << framesThisSecond << "\n";
            framesThisSecond = 0;
            timeSinceLastFpsPrint = 0.0f;

        }
    }
}

void Engine::stop() {
    running = false;
}

long long Engine::getTimeMicro() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}
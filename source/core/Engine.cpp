#include "Engine.h"
#include <GLFW/glfw3.h>

Engine::Engine()
    : bus(new EventBus()),
    scene(new Scene(bus)),
    camera(new Camera(bus)),
    renderer(new Renderer(scene->objectList(), camera)),
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
    scene->initGpu();
    
    renderer->setLights(scene->lightsList());

    
    
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
        renderer->render();

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
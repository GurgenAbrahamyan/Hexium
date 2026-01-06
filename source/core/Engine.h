#pragma once

#include <chrono>
#include <thread>
#include <iostream>

#include "EventBus.h"
#include "Scene.h"
#include "../render/Renderer.h"
#include "PhysicsEngine.h"
#include "../render/UiRender.h"
#include "../input/UIInput.h"
#include "../input/KeyboardInput.h"
#include "EngineContext.h"
#include "../render/camera.h"
#include "../input/MouseInput.h"

class Engine {
public:
    Engine();
    ~Engine();

    void run();
    void stop();
    long long getTimeMicro();

private:
    EventBus* bus;
    Scene* scene;
    Camera* camera;
    Renderer* renderer;
    PhysicsEngine* physicsEngine;
    UiInput* ui;
    UiRender* uiRender;
    MouseInput* mouseInput;
    KeyboardInput* keyboardInput;

    GLFWwindow* window;

    bool running;
    float accumulator;
    const float PHYSICS_STEP = 1.0f / 60.0f;

    int framecount;
    int framesThisSecond;
    float timeSinceLastFpsPrint;
};

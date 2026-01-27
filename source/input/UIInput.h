#pragma once


#include "../core/EventBus.h"
#include "../core/Event.h"
#include "../utils/ModelLoader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../core/EngineContext.h"

class UiInput {
public:
    UiInput(EventBus* bus ) : bus(bus) {
    
        bus->subscribe<CameraMode>([this](CameraMode& event) {
            this->UiMode = !event.key;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            });
    }

    void processInput()
    {
    
     
    }

private:
    ModelLoader* objectLoader;
    EventBus* bus;
    
    GLFWwindow* window = EngineContext::get().getWindow();

    bool UiMode;
};

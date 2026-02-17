#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../math_custom/Mat4.h"
#include "backend/Shader.h"
#include "../core/EngineContext.h"



#include "../render/backend/ShaderManager.h"
#include "../core/EventBus.h"

#include "data/BatchMap.h"
#include "data/MeshBatch.h"
#include "data/ShaderType.h"
#include "data/RenderContext.h"

#include "handlers/RenderHandler.h"
#include "backend/ShaderManager.h"

class Scene;
class Camera;

class Renderer {
public:
    Renderer(EventBus* bus);
    ~Renderer();

    void render(RenderContext* ctx);


    GLFWwindow* getWindow() const;

    
    

private:
    GLFWwindow* window;
   
    ShaderManager* shaderManager;
    EventBus* bus;
    
    std::unordered_map<ShaderType, std::unique_ptr<RenderHandler>> renderHandlers;



    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
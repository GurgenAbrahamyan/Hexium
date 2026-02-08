#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../shapes/Object3D.h"
#include "../render/camera.h"
#include "../math_custom/Mat4.h"
#include "../render/RenderHelpers/Shader.h"
#include "../core/EngineContext.h"
#include "../graphics/resources/RenderMesh.h"
#include "../graphics/resources/Light.h"
#include "../render/GlobalUniformBuffer.h"
#include "../render/RenderHelpers/ShaderManager.h"
#include "../core/EventBus.h"

#include "data/BatchMap.h"
#include "data/MeshBatch.h"
#include "data/ShaderType.h"
#include "data/RenderContext.h"

#include "Renderers/RenderHandler.h"
class Scene;
class Camera;

class Renderer {
public:
    Renderer(EventBus* bus);
    ~Renderer();

    void render(RenderContext* ctx);


    GLFWwindow* getWindow() const;

    
    void markBatchesDirty() { batchesDirty = true; }

private:
    GLFWwindow* window;
    UniformBuffer* globalUBO;
    ShaderManager* shaderManager;
    EventBus* bus;
    
    std::unordered_map<ShaderType, std::unique_ptr<RenderHandler>> renderHandlers;


    bool batchesDirty;

    void rebuildBatches(Scene* scene);
    void updateInstanceData(Scene* scene);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
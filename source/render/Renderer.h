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

struct MeshBatch {
    RenderMesh* mesh;
    std::vector<Mat4> instances;

    MeshBatch() {
        instances.reserve(128); // Pre-allocate
    }
};

using MeshBatchMap = std::unordered_map<RenderMesh*, MeshBatch>;
using MaterialBatchMap = std::unordered_map<Material*, MeshBatchMap>;
using ShaderBatchMap = std::unordered_map<Shader*, MaterialBatchMap>;

class Scene;
class Camera;

class Renderer {
public:
    Renderer(EventBus* bus);
    ~Renderer();

    void render(Scene* scene, Camera* camera);
    void render2(Scene* scene, Camera* camera);

    GLFWwindow* getWindow() const;

    // Call this when objects are added/removed from scene
    void markBatchesDirty() { batchesDirty = true; }

private:
    GLFWwindow* window;
    UniformBuffer* globalUBO;
    ShaderBatchMap renderBatches;
    ShaderManager* shaderManager;
    EventBus* bus;
    Shader* shaderObj;

    bool batchesDirty;

    void rebuildBatches(Scene* scene);
    void updateInstanceData(Scene* scene);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
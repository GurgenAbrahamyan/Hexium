#include "Renderer.h"
#include "../math_custom/GLAdapter.h"
#include "../render/GlobalUniformBuffer.h"
#include "../graphics/data/GlobalUBOData.h"
#include "../core/Scene.h"
#include "Camera.h"

using namespace GLAdapter;

Renderer::Renderer(EventBus* bus)
    : window(nullptr),
    renderBatches(),
    bus(bus),
    shaderManager(nullptr),
    batchesDirty(true)
{
    shaderManager = new ShaderManager(bus);

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        EngineContext::get().windowWidth,
        EngineContext::get().windowHeight,
        "LearnOpenGL",
        nullptr,
        nullptr
    );

    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return;
    }

    EngineContext::get().setWindow(window);

    shaderObj = shaderManager->load(
        "default",
        "resource\\Shaders\\default.vertex",
        "resource\\Shaders\\default.fragment",
        "resource\\Shaders\\default.geom"
    );

    globalUBO = new UniformBuffer(sizeof(GlobalUBOData), 0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Renderer::~Renderer() {
    delete globalUBO;
    delete shaderManager;

    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void Renderer::rebuildBatches(Scene* scene) {
    renderBatches.clear();

    std::vector<Object3D*>& objectList = scene->objectList();


    for (auto* obj : objectList) {
        for (auto& sub : obj->getModel()->getSubMeshes()) {
            if (!sub.mesh || !sub.material) continue;

         
            auto& materialMap = renderBatches[shaderObj];
            auto& meshMap = materialMap[sub.material];
            auto& batch = meshMap[sub.mesh];

            batch.mesh = sub.mesh;
           
        }
    }
}

void Renderer::updateInstanceData(Scene* scene) {
    std::vector<Object3D*>& objectList = scene->objectList();

    // Clear instance data but keep allocated memory
    for (auto& [shader, materialMap] : renderBatches) {
        for (auto& [material, meshMap] : materialMap) {
            for (auto& [mesh, batch] : meshMap) {
                batch.instances.clear();
            }
        }
    }


    for (auto* obj : objectList) {
        for (auto& sub : obj->getModel()->getSubMeshes()) {
            if (!sub.mesh || !sub.material) continue;

            Mat4 model = sub.localTransform *
                GLAdapter::toGL(
                    Mat4::fromMat3(obj->getOrientation()) *
                    Mat4::scale(obj->getScale()) *
                    Mat4::translate(obj->getPosition())
                );

            renderBatches[shaderObj][sub.material][sub.mesh].instances.push_back(model);
        }
    }
}

void Renderer::render(Scene* scene, Camera* camera) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<Light*>& lightsList = scene->lightsList();

    
    if (batchesDirty) {
        rebuildBatches(scene);
        batchesDirty = false;
    }


    updateInstanceData(scene);

  
    shaderObj->Activate();
    shaderObj->setMat4("view", camera->getViewMatrix());
    shaderObj->setMat4("projection", camera->getProjectionMatrix());
    shaderObj->setVec3("cameraPos", camera->getPosition());

    for (Light* light : lightsList) {
        shaderObj->setVec4("lightColor", light->getColor(), 1.0f);
        Vector3 lightPos_engine = light->getWorldPosition();
        Vector3 lightPos_gl = GLAdapter::toGL(lightPos_engine);
        shaderObj->setVec3("lightPos", lightPos_gl);
    }

    for (auto& [shader, materialMap] : renderBatches) {
        // Shader already activated above

        for (auto& [material, meshMap] : materialMap) {
            material->Bind(shader);

            for (auto& [mesh, batch] : meshMap) {
                if (batch.instances.empty()) continue;

                mesh->bind();
                mesh->setupInstanceVBO(batch.instances.size());

                glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVBO());
                glBufferSubData(
                    GL_ARRAY_BUFFER,
                    0,
                    batch.instances.size() * sizeof(Mat4),
                    batch.instances.data()
                );

                glDrawElementsInstanced(
                    GL_TRIANGLES,
                    mesh->indexCount(),
                    GL_UNSIGNED_INT,
                    0,
                    batch.instances.size()
                );
            }
        }
    }

    glfwSwapBuffers(window);
}

GLFWwindow* Renderer::getWindow() const {
    return window;
}

void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

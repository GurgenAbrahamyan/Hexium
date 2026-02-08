#include "Renderer.h"
#include "../math_custom/GLAdapter.h"
#include "../render/GlobalUniformBuffer.h"
#include "../graphics/data/GlobalUBOData.h"
#include "../core/Scene.h"
#include "Camera.h"



using namespace GLAdapter;

Renderer::Renderer(EventBus* bus)
    : window(nullptr),
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

    renderHandlers[ShaderType::OBJECT3D] = std::make_unique<ObjectRenderer>(
        shaderManager->load(
            "default",
            "resource\\Shaders\\object_shader\\default.vertex",
            "resource\\Shaders\\object_shader\\default.fragment",
            "resource\\Shaders\\object_shader\\default.geom",
            ShaderType::OBJECT3D
        )
    );
    renderHandlers[ShaderType::CUBEMAP] = std::make_unique<CubeMapRenderer>(
        shaderManager->load(
            "default_cubemap",
            "resource\\Shaders\\cubemap_shader\\default.vertex",
            "resource\\Shaders\\cubemap_shader\\default.fragment",
            "resource\\Shaders\\object_shader\\default.geom",
            ShaderType::CUBEMAP
        )
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








    void Renderer::render(RenderContext* ctx) {
        // Clear frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        
       

     
        for(auto& [type, handler] : renderHandlers){
         
                handler->renderBatch(*ctx);
            
        }


        glfwSwapBuffers(window);
    }



GLFWwindow* Renderer::getWindow() const {
    return window;
}

void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
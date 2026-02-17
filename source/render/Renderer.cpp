#include "Renderer.h"
#include "../math_custom/GLAdapter.h"




using namespace GLAdapter;

Renderer::Renderer(EventBus* bus)
    : window(nullptr),
    bus(bus),
    shaderManager(nullptr)
  
{
    shaderManager = new ShaderManager(bus);

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return;
    }

    int samples = 8;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, samples);
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

    Shader* objShader = shaderManager->getShader("default");
    objShader->Activate();
    GLuint blockIndex = glGetUniformBlockIndex(objShader->ID, "LightBlock");
    glUniformBlockBinding(objShader->ID, blockIndex, 1);

    renderHandlers[ShaderType::CUBEMAP] = std::make_unique<CubeMapRenderer>(
        shaderManager->load(
            "default_cubemap",
            "resource\\Shaders\\cubemap_shader\\default.vertex",
            "resource\\Shaders\\cubemap_shader\\default.fragment",
            "resource\\Shaders\\object_shader\\default.geom",
            ShaderType::CUBEMAP
        )
    );


   

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
	
    glDepthFunc(GL_LESS);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Renderer::~Renderer() {
   
    delete shaderManager;

    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}








    void Renderer::render(RenderContext* ctx) {
        // Clear frame
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
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
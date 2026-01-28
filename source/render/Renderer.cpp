#include "Renderer.h"
#include "../math_custom/GLAdapter.h"

using namespace GLAdapter;

Renderer::Renderer(std::vector<Object3D*>& objects, Camera* camera)
	: objectList(objects), camera(camera), window(nullptr), shader(nullptr), lightsList(*(new std::vector<Light*>()))
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(EngineContext::get().windowWidth, EngineContext::get().windowHeight, "LearnOpenGL", nullptr, nullptr);
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

    shader = new Shader("resource\\Shaders\\default.vertex", "resource\\Shaders\\default.fragment", "resource\\Shaders\\default.geom");


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);  // ? Try GL_CW instead of GL_CCW due to coordinate flip
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


}
void Renderer::render() {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Activate();

    shader->setMat4("view", camera->getViewMatrix());
    shader->setMat4("projection", camera->getProjectionMatrix());
    shader->setVec3("cameraPos", camera->getPosition());

   

    for (Light* light : lightsList) {
        shader->setVec4("lightColor", light->getColor(), 1.0f);
        Vector3 lightPos_engine = light->getWorldPosition();
        Vector3 lightPos_gl = GLAdapter::toGL(lightPos_engine);
        shader->setVec3("lightPos", lightPos_gl);
    }

    for (auto obj : objectList) {
        for (Model::SubMesh mesh : obj->getModel()->getSubMeshes()) {
            if (!mesh.mesh) continue;
            mesh.mesh->bind();

            
            Mat4 model_engine =
                //Mat4::fromQuat(mesh.rotation) *
                
               

                Mat4::fromMat3(obj->getOrientation())*

               
                
                Mat4::scale(obj->getScale())*
                
               
             
                
                Mat4::translate(obj->getPosition())
             
               
                

                ;

            shader->setMat4("model", mesh.localTransform *
                GLAdapter::toGL(model_engine));

            if (mesh.material) {
                mesh.material->Bind(shader);
            }
            glDrawElements(GL_TRIANGLES, mesh.mesh->indexCount(), GL_UNSIGNED_INT, 0);
        }
    }
    glfwSwapBuffers(window);
}

void Renderer::setObjects(const std::vector<Object3D*>& objects) {
    objectList = objects;
}
void Renderer::setLights(const std::vector<Light*>& lights) {
    
    

    lightsList = lights;
}
GLFWwindow* Renderer::getWindow() const {
    return window;
}

void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
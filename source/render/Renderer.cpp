#include "Renderer.h"

Renderer::Renderer(std::vector<Object3D*>& objects, Camera* camera)
    : objectList(objects), camera(camera), window(nullptr), shader(nullptr)
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

    shader = new Shader("resource\\Shaders\\default.vertex", "resource\\Shaders\\default.fragment");

    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
}

void Renderer::render() {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->Activate();
    shader->setFloat("uTime", glfwGetTime());

    Mat4 view = camera->getViewMatrix();
    Mat4 projection = camera->getProjectionMatrix();

    
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, view.getData());
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, projection.getData());

    for (auto obj : objectList) {
        RenderMesh* mesh = obj->getMesh();
        if (!mesh) continue;

        mesh->bind();
        mesh->update(obj);

        if(obj->getTexture()) {
            obj->getTexture()->Bind();
         //   obj->getTexture()->texUnit(*shader, "tex0", 0);
		}

        Mat4 model = Mat4::translate(obj->getPosition());
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, model.getData());

       
        glDrawElements(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, 0);

        
    }

    glfwSwapBuffers(window);
}

void Renderer::setObjects(const std::vector<Object3D*>& objects) {
    objectList = objects;
}

GLFWwindow* Renderer::getWindow() const {
    return window;
}

void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

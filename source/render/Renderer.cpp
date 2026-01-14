#include "Renderer.h"

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

    shader = new Shader("resource\\Shaders\\default.vertex", "resource\\Shaders\\default.fragment");


    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


}

void Renderer::render() {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    shader->Activate();
    

    Mat4 view = camera->getViewMatrix();
    Mat4 projection = camera->getProjectionMatrix();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    for (Light* light : lightsList) {
        shader->setVec4("lightColor", light->getColor(), 1.0f);
        shader->setVec3("lightPos", light->getWorldPosition());
    }

    for (auto obj : objectList) {
        RenderMesh* mesh = obj->getMesh();

       
        if (!mesh) continue;

	
        mesh->bind();
     
        Mat4 model =
         
           
            Mat4::scale(obj->getScale()) *
            Mat4::fromMat3(obj->getOrientation()) *
            Mat4::translate(obj->getPosition());
            

        shader->setMat4("model", model);

		Vector3 cameraPos = camera->getPosition();

		shader->setVec3("cameraPos", cameraPos);
        

        
    
        
       
        Material* mat = obj->getMaterial();
        if (mat) {
            // Albedo ? tex0
			mat->Bind(shader);

        }



      

       
      //  Vector3 scale = obj->getScale();
       // std::cout << "Obj pos: " << pos.x << "," << pos.y << "," << pos.z << "\n";
       // std::cout << "Obj scale: " << scale.x << "," << scale.y << "," << scale.z << "\n";

        glDrawElements(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, 0);
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
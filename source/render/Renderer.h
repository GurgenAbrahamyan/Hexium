#pragma once

#include <vector>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../shapes/Object3D.h"
#include "../render/camera.h"
#include "../math_custom/Mat4.h"
#include "../render/RenderHelpers/Shader.h"
#include "../core/EngineContext.h"
#include "../graphics/RenderMesh.h"

class Renderer {
public:
    Renderer(std::vector<Object3D*>& objects, Camera* camera);

    void render();
    void setObjects(const std::vector<Object3D*>& objects);
    GLFWwindow* getWindow() const;

private:
    std::vector<Object3D*>& objectList;
    GLFWwindow* window;
    Camera* camera;
    Shader* shader;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

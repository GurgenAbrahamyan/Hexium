#include "Rectangle.h"

StaticMesh* Rectangle::sharedMesh = nullptr;
Material* Rectangle::sharedMaterial = nullptr;
int Rectangle::instanceCount = 0;
bool Rectangle::gpuInitialized = false;

Rectangle::Rectangle(Vector3 pos, Vector3 scale) {
    model = new Model();
    this->position = pos;
    this->scale = scale;

    // Create shared resources on first use
    if (sharedMesh == nullptr) {
        createSharedMesh();
    }

    instanceCount++;

    // All rectangles share the same mesh and material
    model->addSubMesh(
        100,
        sharedMesh,
        sharedMaterial,
        Vector3(0, 0, 0),
        Quat(),
        Vector3(1, 1, 1),
        Mat4()
    );

    // Create physics body (unique per instance)
    float hw = 0.5f, hd = 0.5f, hh = 0.5f;
    std::vector<float> corners = {
        -hw, -hd, -hh,  hw, -hd, -hh,
         hw,  hd, -hh, -hw,  hd, -hh,
        -hw, -hd,  hh,  hw, -hd,  hh,
         hw,  hd,  hh, -hw,  hd,  hh
    };
    std::vector<unsigned int> phys_indices = { 0, 1, 2, 2, 3, 0 };

    physics = new RigidBody(pos, 1.0f, scale, corners, phys_indices);
}

Rectangle::~Rectangle() {
    instanceCount--;

    // Don't delete shared resources here - they're managed separately
    // Just delete the model wrapper (doesn't delete the shared mesh/material)
    delete model;
    delete physics;
}

void Rectangle::createSharedMesh() {
    float hw = 0.5f;  // half width (X)
    float hd = 0.5f;  // half depth (Y) 
    float hh = 0.5f;  // half height (Z)

    std::vector<Vertex> vertices = {
        // Front face (+Y forward direction)
        { Vector3(-hw, hd, -hh), Vector3(1,1,1), Vector2(0,0), Vector3(0,1,0) },
        { Vector3(hw, hd, -hh), Vector3(1,1,1), Vector2(1,0), Vector3(0,1,0) },
        { Vector3(hw, hd,  hh), Vector3(1,1,1), Vector2(1,1), Vector3(0,1,0) },
        { Vector3(-hw, hd,  hh), Vector3(1,1,1), Vector2(0,1), Vector3(0,1,0) },

        // Back face (-Y back direction)
        { Vector3(-hw, -hd, -hh), Vector3(1,1,1), Vector2(0,0), Vector3(0,-1,0) },
        { Vector3(-hw, -hd,  hh), Vector3(1,1,1), Vector2(1,0), Vector3(0,-1,0) },
        { Vector3(hw, -hd,  hh), Vector3(1,1,1), Vector2(1,1), Vector3(0,-1,0) },
        { Vector3(hw, -hd, -hh), Vector3(1,1,1), Vector2(0,1), Vector3(0,-1,0) },

        // Right face (+X right direction)
        { Vector3(hw, -hd, -hh), Vector3(1,1,1), Vector2(0,0), Vector3(1,0,0) },
        { Vector3(hw,  hd, -hh), Vector3(1,1,1), Vector2(1,0), Vector3(1,0,0) },
        { Vector3(hw,  hd,  hh), Vector3(1,1,1), Vector2(1,1), Vector3(1,0,0) },
        { Vector3(hw, -hd,  hh), Vector3(1,1,1), Vector2(0,1), Vector3(1,0,0) },

        // Left face (-X left direction)
        { Vector3(-hw, -hd, -hh), Vector3(1,1,1), Vector2(0,0), Vector3(-1,0,0) },
        { Vector3(-hw, -hd,  hh), Vector3(1,1,1), Vector2(1,0), Vector3(-1,0,0) },
        { Vector3(-hw,  hd,  hh), Vector3(1,1,1), Vector2(1,1), Vector3(-1,0,0) },
        { Vector3(-hw,  hd, -hh), Vector3(1,1,1), Vector2(0,1), Vector3(-1,0,0) },

        // Top face (+Z up direction)
        { Vector3(-hw,  hd,  hh), Vector3(1,1,1), Vector2(0,0), Vector3(0,0,1) },
        { Vector3(hw,  hd,  hh), Vector3(1,1,1), Vector2(1,0), Vector3(0,0,1) },
        { Vector3(hw, -hd,  hh), Vector3(1,1,1), Vector2(1,1), Vector3(0,0,1) },
        { Vector3(-hw, -hd,  hh), Vector3(1,1,1), Vector2(0,1), Vector3(0,0,1) },

        // Bottom face (-Z down direction)
        { Vector3(-hw,  hd, -hh), Vector3(1,1,1), Vector2(0,0), Vector3(0,0,-1) },
        { Vector3(-hw, -hd, -hh), Vector3(1,1,1), Vector2(1,0), Vector3(0,0,-1) },
        { Vector3(hw, -hd, -hh), Vector3(1,1,1), Vector2(1,1), Vector3(0,0,-1) },
        { Vector3(hw,  hd, -hh), Vector3(1,1,1), Vector2(0,1), Vector3(0,0,-1) },
    };

    std::vector<unsigned int> indices = {
        // Front
        0, 1, 2,  2, 3, 0,
        // Back
        4, 6, 5,  6, 4, 7,
        // Right
        8, 9, 10,  10, 11, 8,
        // Left
        12, 13, 14,  14, 15, 12,
        // Top
        16, 17, 18,  18, 19, 16,
        // Bottom
        20, 21, 22,  22, 23, 20,
    };

    sharedMesh = new StaticMesh(vertices, indices);
    sharedMaterial = new Material();
}

void Rectangle::initializeGPU(TextureManager* manager) {
    // Only initialize GPU resources once for all rectangles
    if (!gpuInitialized) {
        sharedMaterial->SetTexture(0, manager->getTexture(manager->addTexture("resource/textures/brick_wall.jpg")));
        sharedMaterial->SetTexture(1, manager->getTexture(manager->addTexture("resource/textures/brick_wall_specular.png")));
        sharedMesh->setupBuffers();
        gpuInitialized = true;
    }
}

void Rectangle::InitializeSharedResources() {
    if (sharedMesh == nullptr) {
        createSharedMesh();
    }
}

void Rectangle::CleanupSharedResources() {
    delete sharedMesh;
    delete sharedMaterial;
    sharedMesh = nullptr;
    sharedMaterial = nullptr;
    gpuInitialized = false;
    instanceCount = 0;
}
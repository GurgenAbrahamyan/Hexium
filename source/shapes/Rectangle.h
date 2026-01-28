#pragma once

#include <vector>
#include "../dynamics/Particle.h"
#include "RigidBody.h"
#include "PhysicsObject.h"
#include "../graphics/DynamicMesh.h"
#include "../graphics/managers/TextureManager.h"

class Rectangle : public Object3D {
public:
    Rectangle(Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1)) {
        model = new Model();
        this->position = pos;
        this->scale = scale;

        // Simple unit cube from -0.5 to 0.5 on all axes
        // Engine coordinates: +X right, +Y forward, +Z up
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

        DynamicMesh* mesh = new DynamicMesh(vertices, indices);
        Material* material = new Material();

        // Add submesh at local position (0,0,0) with no rotation and scale 1
        model->addSubMesh(mesh, material, Vector3(0, 0, 0), Quat(), Vector3(1, 1, 1), Mat4::translate(Vector3(0,0,0))*Mat4::fromQuat(Quat())*Mat4::scale(Vector3(1,1,1)));

        // Create simple physics (no rotation)
        std::vector<float> corners = {
            -hw, -hd, -hh,  hw, -hd, -hh,
             hw,  hd, -hh, -hw,  hd, -hh,
            -hw, -hd,  hh,  hw, -hd,  hh,
             hw,  hd,  hh, -hw,  hd,  hh
        };
        std::vector<unsigned int> phys_indices = { 0, 1, 2, 2, 3, 0 };

        physics = new RigidBody(pos, 1.0f, scale, corners, phys_indices);
    }

    void initializeGPU(TextureManager* manager) override {
        const Model::SubMesh* mesh = &model->getSubMeshes().at(0);
        mesh->material->SetTexture(0, manager->getTexture(manager->addTexture("resource/textures/brick_wall.jpg")));
        mesh->material->SetTexture(1, manager->getTexture(manager->addTexture("resource/textures/brick_wall_specular.png")));
        mesh->mesh->setupBuffers();
    }
};
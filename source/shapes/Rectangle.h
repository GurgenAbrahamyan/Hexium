#pragma once

#include <vector>
#include "../dynamics/Particle.h"
#include "../math_custom/Vector3.h"
#include "RigidBody.h"
#include "PhysicsObject.h"
#include "../graphics/DynamicMesh.h"
#include "../graphics/TextureManager.h"
#include <cstdlib>
#include <ctime>

class Rectangle : public Object3D {
public:

    Rectangle(Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(10, 10, 10), float mass = 1.0f) {
        mesh = nullptr;
        this->mass = mass;
        invMass = 1.0f / mass;
        this->position = pos;
        this->scale = scale;
        float hw = 0.5;
        float hh = 0.5;
        float hd = 0.5;

        std::vector<float> corners = {
            -hw, -hh, -hd,  hw, -hh, -hd,
             hw,  hh, -hd, -hw,  hh, -hd,
            -hw, -hh,  hd,  hw, -hh,  hd,
             hw,  hh,  hd, -hw,  hh,  hd
        };

        


        std::vector<unsigned int> indices = {
            // Front face
            0, 1, 2,  2, 3, 0,
            // Back face
            4, 6, 5,  6, 4, 7,
            // Left face
            8, 9, 10,  10, 11, 8,
            // Right face
            12, 14, 13,  14, 12, 15,
            // Top face
            16, 17, 18,  18, 19, 16,
            // Bottom face
            20, 22, 21,  22, 20, 23
        };

		physics = new RigidBody(pos, mass, scale, corners, indices);
        dynamic_cast<RigidBody*>(physics)->angularVelocity = Vector3(0, 2, 0);

        std::vector<Vertex> vertices = {
            // Front face (+Z)
            {{-hw, -hh,  hd}, {1,1,1}, {0,0}, {0,0,1}},
            {{ hw, -hh,  hd}, {1,1,1}, {1,0}, {0,0,1}},
            {{ hw,  hh,  hd}, {1,1,1}, {1,1}, {0,0,1}},
            {{-hw,  hh,  hd}, {1,1,1}, {0,1}, {0,0,1}},

            // Back face (-Z)
            {{-hw, -hh, -hd}, {1,1,1}, {0,0}, {0,0,-1}},
            {{ hw, -hh, -hd}, {1,1,1}, {1,0}, {0,0,-1}},
            {{ hw,  hh, -hd}, {1,1,1}, {1,1}, {0,0,-1}},
            {{-hw,  hh, -hd}, {1,1,1}, {0,1}, {0,0,-1}},

            // Left face (-X)
            {{-hw, -hh, -hd}, {1,1,1}, {0,0}, {-1,0,0}},
            {{-hw, -hh,  hd}, {1,1,1}, {1,0}, {-1,0,0}},
            {{-hw,  hh,  hd}, {1,1,1}, {1,1}, {-1,0,0}},
            {{-hw,  hh, -hd}, {1,1,1}, {0,1}, {-1,0,0}},

            // Right face (+X)
            {{hw, -hh, -hd}, {1,1,1}, {0,0}, {1,0,0}},
            {{ hw, -hh,  hd}, {1,1,1}, {1,0}, {1,0,0}},
            {{ hw,  hh,  hd}, {1,1,1}, {1,1}, {1,0,0}},
            {{ hw,  hh, -hd}, {1,1,1}, {0,1}, {1,0,0}},
            
            // Top face (+Y)
            {{-hw,  hh,  hd}, {1,1,1}, {0,0}, {0,1,0}},
            {{ hw,  hh,  hd}, {1,1,1}, {1,0}, {0,1,0}},
            {{ hw,  hh, -hd}, {1,1,1}, {1,1}, {0,1,0}},
            {{-hw,  hh, -hd}, {1,1,1}, {0,1}, {0,1,0}},

            // Bottom face (-Y)
            {{-hw, -hh,  hd}, {1,1,1}, {0,0}, {0,-1,0}},
            {{ hw, -hh,  hd}, {1,1,1}, {1,0}, {0,-1,0}},
            {{ hw, -hh, -hd}, {1,1,1}, {1,1}, {0,-1,0}},
            {{-hw, -hh, -hd}, {1,1,1}, {0,1}, {0,-1,0}},
        };

        mesh = new DynamicMesh(vertices, indices);

		material = new Material();
       
    }

    void initializeGPU(TextureManager* manager) override {
        
        if (!material) return;
        material->SetTexture(0, manager->getTexture("resource\\textures\\brick_wall.jpg", TextureUsage::Color));
        material->SetTexture(1, manager->getTexture("resource\\textures\\brick_wall_specular.png", TextureUsage::Color));
       

 


        if (!mesh) return;
           mesh->setupBuffers();

    }

  /*  void createCrate(TextureManager* manager) {
       textures. texture = manager->getTexture("resource\\textures\\crate.jpg");
    }*/
};

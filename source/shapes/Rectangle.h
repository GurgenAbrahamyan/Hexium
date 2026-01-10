#pragma once

#include <vector>

#include "../dynamics/Particle.h"
#include "../math_custom/Vector3.h"
#include "RigidBody.h"
#include "Object3D.h"
#include "../graphics/DynamicMesh.h"
#include "../graphics/TextureManager.h"
#include <cstdlib> 
#include <ctime>



class Rectangle : public RigidBody {
public:


    Rectangle(Vector3 pos = Vector3(0, 0, 0), Vector3 size = Vector3(10, 10, 10), float mass = 1.0f) {

        mesh = nullptr;
        this->mass = mass;
		invMass = 1.0f / mass;
        this->position = pos;
       

       float hw = size.getX() / 2.0f;
        float hh = size.getY() / 2.0f;
        float hd = size.getZ() / 2.0f;

       


        std::vector<float> corners = {
            -hw, -hh, -hd, hw, -hh, -hd,
            hw,  hh, -hd, -hw,  hh, -hd,
            -hw, -hh,  hd, hw, -hh,  hd,
            hw,  hh,  hd, -hw,  hh,  hd
        };

        this->particles = new ParticleSystem(mass);
		orientation.setIdentity();
        for (int x = 0; x < corners.size();x+=3 ) {
            Particle* p = new Particle(mass / corners.size()/3, Vector3( corners[x], corners[x+1], corners[x+2]));
            particles->addParticle(p);
        }


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

        std::vector<Vertex> vertices = {
            // Front face
            {{-hw, -hh,  hd}, {1,0,0}, {0,0}}, // Red
            {{ hw, -hh,  hd}, {0,1,0}, {1,0}}, // Green
            {{ hw,  hh,  hd}, {0,0,1}, {1,1}}, // Blue
            {{-hw,  hh,  hd}, {1,1,0}, {0,1}}, // Yellow

            // Back face
            {{-hw, -hh, -hd}, {1,0,0}, {0,0}},
            {{ hw, -hh, -hd}, {0,1,0}, {1,0}},
            {{ hw,  hh, -hd}, {0,0,1}, {1,1}},
            {{-hw,  hh, -hd}, {1,1,0}, {0,1}},

            // Left face
            {{-hw, -hh, -hd}, {1,0,0}, {0,0}},
            {{-hw, -hh,  hd}, {0,1,0}, {1,0}},
            {{-hw,  hh,  hd}, {0,0,1}, {1,1}},
            {{-hw,  hh, -hd}, {1,1,0}, {0,1}},

            // Right face
            {{ hw, -hh, -hd}, {1,0,0}, {0,0}},
            {{ hw, -hh,  hd}, {0,1,0}, {1,0}},
            {{ hw,  hh,  hd}, {0,0,1}, {1,1}},
            {{ hw,  hh, -hd}, {1,1,0}, {0,1}},

            // Top face
            {{-hw,  hh,  hd}, {1,0,0}, {0,0}},
            {{ hw,  hh,  hd}, {0,1,0}, {1,0}},
            {{ hw,  hh, -hd}, {0,0,1}, {1,1}},
            {{-hw,  hh, -hd}, {1,1,0}, {0,1}},

            // Bottom face
            {{-hw, -hh,  hd}, {1,0,0}, {0,0}},
            {{ hw, -hh,  hd}, {0,1,0}, {1,0}},
            {{ hw, -hh, -hd}, {0,0,1}, {1,1}},
            {{-hw, -hh, -hd}, {1,1,0}, {0,1}},
        };


		mesh = new DynamicMesh(vertices, indices);
        
	   // texture = new Texture("D:\\C++ Development\\Hexium\\resource\\textures\\crate.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        
        srand(time(0));       

        float x = rand() / (float)RAND_MAX;   
        float y = rand() / (float)RAND_MAX;
        float z = rand() / (float)RAND_MAX;

        Vector3 col(x, y, z);
        mesh->setColor(col);

        angularVelocity = Vector3(0, 2, 0 );
    }

    void initializeGPU(TextureManager* manager) override {
       
		

       
		texture = manager->getTexture("resource\\textures\\crate2.jpg");
        
        mesh->setupBuffers();
        

      
    }

    void createCrate(TextureManager* manager) {
		texture = manager->getTexture("resource\\textures\\crate.jpg");
    }
};

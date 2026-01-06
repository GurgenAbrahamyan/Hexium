#pragma once

#include <vector>

#include "../dynamics/Particle.h"
#include "../math_custom/Vector3.h"
#include "RigidBody.h"
#include "Object3D.h"
#include "../graphics/DynamicMesh.h"
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


        std::vector< int> indices = {
            0,1,2, 2,3,0,
            4,5,6, 6,7,4,
            0,4,7, 7,3,0,
            1,5,6, 6,2,1,
            0,1,5, 5,4,0,
            3,2,6, 6,7,3
        };

        
		mesh = new DynamicMesh(particles, indices);

	    texture = new Texture("resources/textures/crate.jpg");

        srand(time(0));       

        float x = rand() / (float)RAND_MAX;   
        float y = rand() / (float)RAND_MAX;
        float z = rand() / (float)RAND_MAX;

        Vector3 col(x, y, z);
        mesh->setColor(col);

        angularVelocity = Vector3(0, 0, 2.0f);
    }
};

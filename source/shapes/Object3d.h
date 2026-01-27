#pragma once

#include <vector>
#include "../graphics/RenderMesh.h"
#include "../graphics/Material.h"
#include "../graphics/managers/TextureManager.h"
#include "../shapes/PhysicsObject.h"
#include "../shapes/RigidBody.h"
#include "../math_custom/Vector3.h"
#include "../dynamics/ForceGenerator.h"
#include "../math_custom/Quat.h"
#include "../graphics/Model.h"

class Object3D {
protected:
    PhysicsObject* physics;
	Model* model;

    Vector3 scale = Vector3(0.5, 0.5, 0.5);
    Vector3 position = Vector3(0, 0, 0);
    float mass = 1.0f;
    float invMass = 1.0f;


public:
    Object3D(PhysicsObject* physicsObj = nullptr, Model* model = nullptr)
        : physics(physicsObj), model(model)
    {



        if (!physics) {
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

            physics = new RigidBody(Vector3(0, 0, 0), mass, scale, corners, indices);
        }
    }

    virtual ~Object3D() {
        delete physics;
        delete model;
    }


    virtual void initializeGPU(TextureManager* manager) {
       
        for(auto mesh : model->getSubMeshes()) {
            if (mesh.mesh) {
                mesh.mesh->setupBuffers();
            }
		}
    }


    virtual void applyForce(ForceGenerator* gen) {
        if (physics) physics->applyForce(gen);
    }

    Vector3 getPosition() const {
        return physics ? Vector3(physics->position.x, physics->position.y, physics->position.z) : Vector3(0, 0, 0);
    }
    void setPosition(const Vector3& pos) {
        if (physics) {
            physics->position = pos;
        }
    }

    Matrix3 getOrientation() {
        return physics->getOrientation();
    }

        Vector3 getScale() const {
            return scale;
        }
        void setScale(const Vector3 & s) {
            scale = s;
        }

        Model* getModel() {
            return model;
		}
        PhysicsObject* getPhysics() {
            return physics;
        }

    };

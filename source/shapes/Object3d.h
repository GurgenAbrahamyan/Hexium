#pragma once

#include <vector>
#include "../graphics/RenderMesh.h"
#include "../graphics/Material.h"
#include "../graphics/TextureManager.h"
#include "../shapes/PhysicsObject.h"
#include "../math_custom/Vector3.h"
#include "../dynamics/ForceGenerator.h"
#include "../math_custom/Mat3.h"


class Object3D {
protected:
    PhysicsObject* physics = nullptr;  
    RenderMesh* mesh = nullptr;       
    Material* material = nullptr;      

	Vector3 scale = Vector3(1, 1, 1);
	Vector3 position = Vector3(0, 0, 0);
	float mass = 1.0f;
	float invMass = 1.0f;


public:
    Object3D(PhysicsObject* physicsObj = nullptr, RenderMesh* meshObj = nullptr, Material* mat = nullptr)
        : physics(physicsObj), mesh(meshObj), material(mat)
    {
    }

    virtual ~Object3D() {
        delete physics;   
        delete mesh;      
        delete material;  
    }

   
    virtual void initializeGPU(TextureManager * manager) {
       
        if (mesh) mesh->setupBuffers();
    }
    

    virtual void applyForce(ForceGenerator* gen) {
        if (physics) physics->applyForce(gen);
    }

    Vector3 getPosition() const {
        return physics ? physics->position : Vector3(0, 0, 0);
    }

    Matrix3 getOrientation() const {
        return physics->getOrientation();
    }

    Vector3 getScale() const {
        return scale;
    }

    RenderMesh* getMesh() const { return mesh; }
    Material* getMaterial() const { return material; }

    PhysicsObject* getPhysics() {      return physics;
    }
  
};

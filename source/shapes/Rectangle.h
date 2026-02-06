#pragma once

#include <vector>
#include "../dynamics/Particle.h"
#include "RigidBody.h"
#include "PhysicsObject.h"
#include "Object3d.h"
#include "../graphics/resources/StaticMesh.h"
#include "../graphics/managers/TextureManager.h"

class Rectangle : public Object3D {
public:
    Rectangle(Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1));
    ~Rectangle();

    void initializeGPU(TextureManager* manager) override;

    // Call this once before creating any Rectangles
    static void InitializeSharedResources();
    // Call this once after deleting all Rectangles
    static void CleanupSharedResources();

private:
    static StaticMesh* sharedMesh;
    static Material* sharedMaterial;
    static int instanceCount;
    static bool gpuInitialized;

    static void createSharedMesh();
};
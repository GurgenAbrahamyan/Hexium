#pragma once


#include <vector>
#include "../dynamics/Particle.h"
#include "../math_custom/Vector3.h"
#include "../dynamics/ForceGenerator.h"
#include "../graphics/Texture.h"
#include "../graphics/TextureManager.h"

class ForceGenerator;
class RenderMesh;

class Object3D {

protected:
    ParticleSystem* particles = nullptr;
    RenderMesh* mesh = nullptr;
    Texture* texture =nullptr;  //should be changed to texture array in future
     
    Vector3 position;
public:
    Object3D(Vector3 pos = Vector3(10, 10, 10), float mass = 1.0f)
        : mesh(nullptr), position(pos)
    {
        position = pos;
        particles = new ParticleSystem(mass);
    }

    virtual ~Object3D() {
        delete particles;
        delete mesh;
    }



    virtual void initializeGPU(TextureManager* manager) {}

    RenderMesh* getMesh() const { return mesh; }
    ParticleSystem* getParticleSystem() const { return particles; }
    Vector3 getPosition() const { return position; }
    
    Texture* getTexture() const { return texture; }
	void SetTexture(Texture* tex) { texture = tex; }

    virtual void applyForce(ForceGenerator* gen, float dt) {}
    virtual void integrate(float dt) {}

};

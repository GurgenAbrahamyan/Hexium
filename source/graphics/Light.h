
#pragma once
#pragma once
#include "../math_custom/Vector3.h"
#include "../graphics/RenderMesh.h"
#include "../shapes/Object3D.h"
#include <vector>

enum class LightType {
    Point,
    Spot,
    Directional
};

class Light {
private:
    Object3D* owner = nullptr;
    
    LightType type = LightType::Point;

    
    Vector3 color = Vector3(1, 1, 1);
	Vector3 pos = Vector3(0, 0, 0);
    float intensity = 1.0f;




public:

    Light(Vector3 color, Vector3 pos, float intensity, LightType type) :
        color(color), pos(pos), intensity(intensity), type(type){}
   


    
    Vector3 getWorldPosition() const {
        if (!owner) return pos;

   
        Matrix3 invRot = owner->getOrientation().transpose();


        Vector3 scaled = pos * (owner->getScale());

        Vector3 final = (invRot * scaled).add(owner->getPosition());

        return final;

        return final;
    }

    
    void setOwner(Object3D* obj) {
        owner = obj;
    
    }

    void setPosition(const Vector3& position) {
        pos = position;
	}   
	LightType getType() const { return type; }
    Vector3 getColor() const { return color; }
	float getIntensity() const { return intensity; }

};
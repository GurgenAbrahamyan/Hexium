
#pragma once
#pragma once
#include "../math_custom/Vector3.h"

#include "../shapes/Object3D.h"
#include <vector>

enum class LightType {
    Point = 0,
    Spot = 2,
    Directional = 1
};

class Light {
private:
    Object3D* owner = nullptr;
    
    LightType type = LightType::Point;

    
    Vector3 color = Vector3(1, 1, 1);
	Vector3 pos = Vector3(0, 0, 0);
    Vector3 direction = Vector3(0, 0, -1);  
    float intensity = 1.0f;
    float innerConeAngle = 12.5f;  
    float outerConeAngle = 17.5f;




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

    Vector3 getDirection() const {
        if (!owner) return direction;

        
        Matrix3 invRot = owner->getOrientation().transpose();
        return (invRot * direction).normalize();
    }

  
    float getInnerCone() const {
        return cos(innerConeAngle * 3.14159f / 180.0f);
    }

    float getOuterCone() const {
        return cos(outerConeAngle * 3.14159f / 180.0f);
    }

  
    void setDirection(const Vector3& dir) {
        direction = dir.normalize();
    }

    void setConeAngles(float inner, float outer) {
        innerConeAngle = inner;
        outerConeAngle = outer;
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
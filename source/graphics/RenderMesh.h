#pragma once

#include "../math_custom/Vector3.h"

class Object3D;

class RenderMesh {
public:
    virtual ~RenderMesh() = default;

    virtual void bind() = 0;
    virtual void update(Object3D* obj) = 0; 
    virtual void setupBuffers() = 0;
    virtual int indexCount() const = 0;

    virtual void setColor(const Vector3& col) =0;
  

protected:
   
};
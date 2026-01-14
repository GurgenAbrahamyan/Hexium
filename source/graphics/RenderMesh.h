#pragma once

#include "../math_custom/Vector3.h"
#include <vector>
#include "Vertex.h"


class Object3D;

class RenderMesh {
public:
    virtual ~RenderMesh() = default;

    virtual void bind() = 0;
 
    virtual void setupBuffers() = 0;
    virtual int indexCount() const = 0;

    virtual void setColor(const Vector3& col) =0;
  
    virtual Vertex& getVertex(int i) {
        return vertices[i];  
    }
    std::vector<Vertex>& getVertexes() {
        return vertices;
    }


protected:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
#pragma once

#include "../../math_custom/Vector3.h"
#include <vector>
#include "../data/Vertex.h"
#include "../../math_custom/Mat4.h"

#include "glad/glad.h"

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
    void setID(int i) {
        ID = i;
    }
    int getID() {
       return ID;
    }

    virtual void setupInstanceVBO(size_t maxInstances) {};
    virtual void updateInstanceVBO(const std::vector<Mat4>& matrices) {};
    virtual GLuint getInstanceVBO() { return 0; };
    



protected:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

   
    int ID = -1;

   
};
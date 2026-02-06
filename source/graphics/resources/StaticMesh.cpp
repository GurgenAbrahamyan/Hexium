#include "StaticMesh.h"
#include <iostream>
#include "../../math_custom/Mat4.h"

StaticMesh::StaticMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& ind)
    : vertices(vertices), indices(ind), instanceVBO(0), instanceVBOCapacity(0)
{
}

StaticMesh::~StaticMesh() {
    delete VAO1;
    delete VBO1;
    delete EBO1;

    if (instanceVBO != 0) {
        glDeleteBuffers(1, &instanceVBO);
    }
}

void StaticMesh::bind() {
    if (VAO1) {
        VAO1->Bind();
    }
}

int StaticMesh::indexCount() const {
    return static_cast<int>(indices.size());
}

void StaticMesh::setupBuffers() {
    if (VAO1) return; // Already initialized

    std::cout << "Setting up buffers\n";

    VAO1 = new VAO();
    VAO1->Bind();

    VBO1 = new VBO(vertices.data(), vertices.size() * sizeof(Vertex), false);
    EBO1 = new EBO(indices.data(), indices.size() * sizeof(unsigned int), false);

    // Setup vertex attributes
    VAO1->LinkAttrib(*VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);                              // position
    VAO1->LinkAttrib(*VBO1, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));        // color
    VAO1->LinkAttrib(*VBO1, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));           // uv
    VAO1->LinkAttrib(*VBO1, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));       // normal

    VAO1->Unbind();
}

void StaticMesh::setupInstanceVBO(size_t instanceCount) {
    // Only reallocate if we need more space
    if (instanceCount <= instanceVBOCapacity) {
        return;
    }

    // Make sure VAO is set up first
    if (!VAO1) {
        setupBuffers();
    }

    
    size_t newCapacity = instanceCount + (instanceCount / 2);

    if (instanceVBO == 0) {
        glGenBuffers(1, &instanceVBO);
    }

    VAO1->Bind();

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        newCapacity * sizeof(Mat4),
        nullptr,
        GL_DYNAMIC_DRAW
    );


   
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(4 + i);
        glVertexAttribPointer(
            4 + i,                      
            4,                         
            GL_FLOAT,                   
            GL_FALSE,                   
            sizeof(Mat4),               
            (void*)(sizeof(float) * 4 * i)  
        );
        glVertexAttribDivisor(4 + i, 1);  
    }

    VAO1->Unbind();

    instanceVBOCapacity = newCapacity;
}

GLuint StaticMesh::getInstanceVBO() {
    return instanceVBO;
}

void StaticMesh::setColor(const Vector3& col) {
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].color.setX(col.getX());
        vertices[i].color.setY(col.getY());
        vertices[i].color.setZ(col.getZ());
    }

    // Update VBO if already initialized
    if (VBO1) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO1->ID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    }
}
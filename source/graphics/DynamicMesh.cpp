#include "DynamicMesh.h"
#include <iostream>


DynamicMesh::DynamicMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& ind)
{
	this->vertices = vertices;
    this->indices = ind;
}

DynamicMesh::~DynamicMesh() {
    delete VAO1;
    delete VBO1;
    delete EBO1;
}

void DynamicMesh::bind() {
    VAO1->Bind();
}




int DynamicMesh::indexCount() const {
    return static_cast<int>(indices.size());
}

void DynamicMesh::setupBuffers()
{

    std::cout << "settin buffers";
    VAO1 = new VAO();
    VAO1->Bind();

    VBO1 = new VBO(vertices.data(), vertices.size() * sizeof(Vertex),
        false);

    EBO1 = new EBO(indices.data(), indices.size() * sizeof(unsigned int), false);

    VAO1->LinkAttrib(*VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO1->LinkAttrib(*VBO1, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
    VAO1->LinkAttrib(*VBO1, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    VAO1->LinkAttrib(*VBO1, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));


    VAO1->Unbind();
}

void DynamicMesh::setColor(const Vector3& col) {
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].color.setX(col.getX());
        vertices[i].color.setY(col.getY());
        vertices[i].color.setZ(col.getZ());
    }
}

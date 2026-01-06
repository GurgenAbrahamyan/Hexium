#include "DynamicMesh.h"

DynamicMesh::DynamicMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& ind)
    : vertices(vertices), indices(ind)
{
   
}

DynamicMesh::~DynamicMesh() {
    delete VAO1;
    delete VBO1;
    delete EBO1;
}

void DynamicMesh::bind() {
    VAO1->Bind();
}

void DynamicMesh::update(Object3D* obj) {
    int numVertices = static_cast<int>(vertices.size());
    float* buffer = new float[numVertices * 8]; 

    for (int i = 0; i < numVertices; i++) {
        Vector3 localPos = vertices[i].position;
        Vector3 worldPos = localPos;

        if (RigidBody* rb = dynamic_cast<RigidBody*>(obj)) {
            worldPos = rb->getOrientation() * localPos;
            worldPos = worldPos.add(rb->getPosition());
        }
        else if (obj) {
            worldPos = worldPos.add(obj->getPosition());
        }

        
        buffer[i * 8 + 0] = worldPos.x;
        buffer[i * 8 + 1] = worldPos.y;
        buffer[i * 8 + 2] = worldPos.z;

        buffer[i * 8 + 3] = vertices[i].color.x;
        buffer[i * 8 + 4] = vertices[i].color.y;
        buffer[i * 8 + 5] = vertices[i].color.z;

        buffer[i * 8 + 6] = vertices[i].uv[0];
        buffer[i * 8 + 7] = vertices[i].uv[1];

    }

    VBO1->UpdateData(buffer, numVertices * 8 * sizeof(float));
    delete[] buffer;
}


int DynamicMesh::indexCount() const {
    return static_cast<int>(indices.size());
}

void DynamicMesh::setupBuffers()
{
    VAO1 = new VAO();
    VAO1->Bind();

    VBO1 = new VBO(vertices.data(), vertices.size() * sizeof(Vertex),
        true);

    EBO1 = new EBO(indices.data(),  indices.size() * sizeof(unsigned int), true);

    VAO1->LinkAttrib(*VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO1->LinkAttrib(*VBO1, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
    VAO1->LinkAttrib(*VBO1, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    

    VAO1->Unbind();
}

  void DynamicMesh::setColor(const Vector3& col) {
      for (int i = 0; i < vertices.size(); i++) {
          vertices[i].color.setX(col.getX());
          vertices[i].color.setY(col.getY());
          vertices[i].color.setZ(col.getZ());
	  }
  }


#include "DynamicMesh.h"

DynamicMesh::DynamicMesh(ParticleSystem* ps, const std::vector<int>& ind)
    : ps(ps), indices(ind)
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
    int numParticles = ps->getNumParticles();
    float* positions = new float[numParticles * 3];

    for (int i = 0; i < numParticles; i++) {
        Vector3 localPos = ps->particles[i]->position;
        Vector3 worldPos = localPos;

        if (RigidBody* rb = dynamic_cast<RigidBody*>(obj)) {
            worldPos = rb->getOrientation() * localPos;
            worldPos = worldPos.add(rb->getPosition());
        }
        else if (obj) {
            worldPos = worldPos.add(obj->getPosition());
        }

        positions[i * 3 + 0] = worldPos.x;
        positions[i * 3 + 1] = worldPos.y;
        positions[i * 3 + 2] = worldPos.z;
    }

    VBO1->UpdateData(positions, sizeof(float) * numParticles * 3);
    delete[] positions;
}

int DynamicMesh::indexCount() const {
    return static_cast<int>(indices.size());
}

void DynamicMesh::setupBuffers() {
    VAO1 = new VAO();
    VAO1->Bind();

    VBO1 = new VBO(ps->getPositionArrayForMesh(), ps->getNumParticles() * 3 * sizeof(float), true);
    EBO1 = new EBO(reinterpret_cast<GLuint*>(indices.data()), indices.size() * sizeof(int), true);

    VAO1->LinkVBO(VBO1, 0);
    VAO1->Unbind();
    VBO1->Unbind();
    EBO1->Unbind();
}

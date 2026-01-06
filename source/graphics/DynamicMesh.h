#pragma once
#include "RenderMesh.h"
#include <vector>
#include "../math_custom/Vector3.h"
#include "../render/RenderHelpers/containers/VAO.h"
#include "../render/RenderHelpers/containers/VBO.h"
#include "../render/RenderHelpers/containers/EBO.h"
#include "Vertex.h"
#include "../shapes/Object3D.h"
#include "../shapes/RigidBody.h"


class DynamicMesh : public RenderMesh {
public:
    DynamicMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& ind);
    ~DynamicMesh();

    void bind() override;
    void update(Object3D* obj) override;
    int indexCount() const override;
    void setColor(const Vector3& col) override;

private:
    void setupBuffers() override;

    std::vector<unsigned int> indices;
    VAO* VAO1 = nullptr;
    VBO* VBO1 = nullptr;
    EBO* EBO1 = nullptr;
    std::vector<Vertex> vertices;
};

#pragma once


#include <vector>
#include "../math_custom/Vector3.h"
#include "../render/RenderHelpers/containers/VAO.h"
#include "../render/RenderHelpers/containers/VBO.h"
#include "../render/RenderHelpers/containers/EBO.h"



#include "RenderMesh.h"
#include "../math_custom/Vector3.h"
#include <vector>
#include "Vertex.h"

class DynamicMesh : public RenderMesh {
public:
    DynamicMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~DynamicMesh() override;

    void bind() override;
    void setupBuffers() override;
    int indexCount() const override;
    void setColor(const Vector3& col) override;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    VAO* VAO1 = nullptr;
    VBO* VBO1 = nullptr;
    EBO* EBO1 = nullptr;
};


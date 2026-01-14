#pragma once
#include "RenderMesh.h"
#include <vector>
#include "../render/RenderHelpers/containers/VAO.h"
#include "../render/RenderHelpers/containers/VBO.h"
#include "../render/RenderHelpers/containers/EBO.h"

class StaticMesh : public RenderMesh {
public:
    StaticMesh(float* positions, const std::vector<unsigned int>& ind) {
        indexCnt = static_cast<int>(ind.size());

        VAO1 = new VAO();
        //VBO1 = new VBO(const_cast<GLfloat*>(positions), positions.size() * sizeof(float), false);
        EBO1 = new EBO(const_cast<GLuint*>(ind.data()), ind.size() * sizeof(unsigned int), false);

        VAO1->Bind();
        VAO1->LINKVBO(VBO1, 0);
        VAO1->Unbind();
    }

    ~StaticMesh() {
        delete VAO1;
        delete VBO1;
        delete EBO1;
    }

    void bind() override { VAO1->Bind(); }
    
    int indexCount() const override { return indexCnt; }

private:
    VAO* VAO1 = nullptr;
    VBO* VBO1 = nullptr;
    EBO* EBO1 = nullptr;
    int indexCnt;
};
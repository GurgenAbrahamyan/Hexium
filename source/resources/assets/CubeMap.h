#pragma once
#include <glad/glad.h>
#include "../../render/backend/containers/VAO.h"
#include "../../render/backend/containers/VBO.h"


class CubeMap {
public:
   

    void bind() const;
    void unbind() const;
    void bindTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
    }
    void setTexture(GLuint textureID) { texID = textureID; }
    CubeMap();
    ~CubeMap();

	
private:
    VAO* VAO1;
    VBO* VBO1;

    GLuint texID = 0;
};

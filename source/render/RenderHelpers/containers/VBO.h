#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO {
public:
    GLuint ID;

    VBO(GLfloat* vertices, GLsizeiptr size, bool dynamic) {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    ~VBO() { glDeleteBuffers(1, &ID); }

    void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, ID); }
    void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    void UpdateData(GLfloat* newVertices, GLsizeiptr size) {
        Bind();  
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, newVertices);
    }
};

#endif

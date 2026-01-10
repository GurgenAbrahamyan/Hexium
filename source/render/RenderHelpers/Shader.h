#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexFile, const char* fragmentFile);
    ~Shader();

    void Activate() const;
    void setFloat(const char* name, float value);


private:
    GLuint compileShader(const char* source, GLenum type);
    void checkLinkErrors(GLuint program);
};

#endif

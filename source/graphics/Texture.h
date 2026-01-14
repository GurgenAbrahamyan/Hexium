#pragma once

#include <../glad/glad.h>
#include <cstdint>



enum TextureUsage {
    Color,      
    Specular,   
    Normal,
    Data       
};





struct TextureDesc {
    GLenum target = GL_TEXTURE_2D;
    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;

    bool generateMipmaps = true;
    GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLenum magFilter = GL_LINEAR;
    GLenum wrapS = GL_REPEAT;
    GLenum wrapT = GL_REPEAT;
};


class Texture {
public:
   
    Texture(int width, int height, const void* data, const TextureDesc& desc);
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    void Bind( int slot = 0);
    void Unbind() const;
    void Delete(); 

    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    GLuint ID = 0;
    GLenum target = GL_TEXTURE_2D;
    int width = 0;
    int height = 0;
};

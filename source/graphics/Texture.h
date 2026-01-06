#pragma once

#include "../stb/stb_image.h"
#include <../glad/glad.h>
#include <string>
#include "../render/RenderHelpers/Shader.h"



class Texture
{
public:
	GLuint ID = 0;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	
	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	void Bind();
	
	void Unbind();

	void Delete();
};

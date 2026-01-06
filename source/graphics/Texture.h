#pragma once

#include "../stb/stb_image.h"
#include <../glad/glad.h>
#include <string>
class Texture {

private:

	int imgWidth, imgHeight, numChannels;
	unsigned char* data;

	GLuint texture;
	
public:
	Texture(std::string path) {

		data = stbi_load(path.c_str(), &imgWidth, &imgHeight, &numChannels, 0);

		glGenTextures(1, &texture);

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~Texture() {
		stbi_image_free(data);
		glDeleteTextures(1, &texture);
	}
};
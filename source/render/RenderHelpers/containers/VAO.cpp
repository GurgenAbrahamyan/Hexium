#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include "VBO.h"

class VAO
{
public:

	GLuint ID;
	VAO() {
		glGenVertexArrays(1, &ID);

	}

	void LinkVBO(VBO* VBO, GLuint layout) {
		VBO->Bind();
		glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(layout);
		VBO->Unbind();
	}
	void Bind() {
		glBindVertexArray(ID);
	}

	void Unbind() {
		glBindVertexArray(0);
	}

	void Delete() {
		glDeleteVertexArrays(1, &ID);
	}
};

#endif
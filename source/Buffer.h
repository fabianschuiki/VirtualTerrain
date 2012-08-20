/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include <SFML/OpenGL.hpp>


class Buffer
{
public:
	const GLenum type;
	GLuint buffer;
	
	Buffer(GLenum type);
	~Buffer();
	
	void bind();
	void unbind();
	
	void loadData(GLsizei length, const GLvoid *data, GLenum usage);
};
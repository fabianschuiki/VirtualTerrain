/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include <SFML/OpenGL.hpp>


class Texture
{
public:
	GLuint texture;
	GLint width;
	GLint height;
	GLenum internalFormat;
	GLenum format;
	
	Texture();
	~Texture();
	
	void bind();
	static void unbind();
	
	void loadImage(GLint width, GLint height, GLenum internalFormat, GLenum format, GLenum type, const void *data);
};
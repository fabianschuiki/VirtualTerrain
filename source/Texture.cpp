/*
 * Copyright © 2012 Fabian Schuiki
 */

#define TARGET GL_TEXTURE_2D

#include <iostream>
#include "Texture.h"


Texture::Texture()
{
	glGenTextures(1, &texture);
	bind();
	glTexParameterf(TARGET, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(TARGET, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(TARGET, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void Texture::bind()
{
	glBindTexture(TARGET, texture);
}

void Texture::unbind()
{
	glBindTexture(TARGET, 0);
}

void Texture::loadImage(GLint width, GLint height, GLenum internalFormat, GLenum format, GLenum type, const void *data)
{
	this->width = width;
	this->height = height;
	this->internalFormat = internalFormat;
	this->format = format;
	
	bind();
	glTexImage2D(TARGET, 0, internalFormat, width, height, 0, format, type, data);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "unable to create texture " << width << "x" << height << " (internal " << (int)internalFormat << ", format " << (int)format << "), error " << (int)err << std::endl;
}
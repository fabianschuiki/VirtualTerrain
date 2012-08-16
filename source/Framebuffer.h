/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include <SFML/OpenGL.hpp>


class Framebuffer
{
public:
	const GLint width;
	const GLint height;
	GLuint buffer;
	
	Framebuffer(GLint width, GLint height);
	~Framebuffer();
	
	void bind();
	static void unbind();
	bool validate();
	
	void attachTexture(GLenum attachment, GLenum textureTarget, GLuint texture);
};
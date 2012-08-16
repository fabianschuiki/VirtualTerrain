/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include <SFML/OpenGL.hpp>


class Framebuffer
{
public:
	GLuint buffer;
	
	Framebuffer();
	~Framebuffer();
	
	void bind();
	static void unbind();
	bool validate();
	
	void attachTexture(GLenum attachment, GLenum textureTarget, GLuint texture);
};
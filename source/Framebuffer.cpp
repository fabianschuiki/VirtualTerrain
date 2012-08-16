/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <iostream>
#include "Framebuffer.h"


Framebuffer::Framebuffer()
{
	glGenFramebuffersEXT(1, &buffer);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffersEXT(1, &buffer);
}

void Framebuffer::bind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, buffer);
}

void Framebuffer::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

bool Framebuffer::validate()
{
	bind();
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status == GL_FRAMEBUFFER_COMPLETE_EXT)
		return true;
	std::cerr << "*** framebuffer is incomplete: " << (int)status << std::endl;
	return false;
}

void Framebuffer::attachTexture(GLenum attachment, GLenum textureTarget, GLuint texture)
{
	bind();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, textureTarget, texture, 0);
}
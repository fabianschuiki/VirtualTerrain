/*
 * Copyright © 2012 Fabian Schuiki
 */

#include "Buffer.h"


Buffer::Buffer(GLenum type)
:   type(type)
{
	glGenBuffers(1, &buffer);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &buffer);
}


void Buffer::bind()
{
	glBindBuffer(type, buffer);
}

void Buffer::unbind()
{
	glBindBuffer(type, 0);
}


void Buffer::loadData(GLsizei length, const GLvoid *data, GLenum usage)
{
	bind();
	glBufferData(type, length, data, usage);
}
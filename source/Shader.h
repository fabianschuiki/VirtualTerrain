/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include <SFML/OpenGL.hpp>


class Shader
{
public:
	const GLenum type;
	char* const path;
	GLchar* source;
	GLuint shader;
	bool valid;
	
	Shader(GLenum type, const char* path);
	~Shader();
	
	void compile();
};
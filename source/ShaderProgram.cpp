/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <SFML/OpenGL.hpp>
#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const char* vert_path, const char* frag_path)
:   vertexShader(GL_VERTEX_SHADER, vert_path),
    fragmentShader(GL_FRAGMENT_SHADER, frag_path)
{
	program = glCreateProgram();
	reload();
}

ShaderProgram::~ShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(program);
}


void ShaderProgram::reload()
{
	//Compile both shaders.
	vertexShader.compile();
	fragmentShader.compile();
	
	//Attach and link.
	glAttachShader(program, vertexShader.shader);
	glAttachShader(program, fragmentShader.shader);
	glLinkProgram(program);
}

void ShaderProgram::use()
{
	glUseProgram(program);
}
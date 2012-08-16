/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <iostream>
#include <fstream>
#include "Shader.h"


Shader::Shader(GLenum type, const char* path)
:    type(type), path(strdup(path))
{
	shader = glCreateShader(type);
	source = NULL;
	valid = false;
}

Shader::~Shader()
{
	free(path);
	if (source)
		delete source;
	glDeleteShader(shader);
}


void Shader::compile()
{
	std::cout << "loading shader " << path << "... ";
	
	std::ifstream f(path);
	if (!f.good()) {
		std::cerr << "*** unable to read shader " << path << std::endl;
		valid = false;
		return;
	}
	f.seekg(0, std::ios::end);
	GLint length = f.tellg();
	f.seekg(0, std::ios::beg);
	
	if (source) delete source;
	source = new GLchar[length+1];
	int i;
	for (i = 0; f.good(); i++)
		source[i] = f.get();
	source[i] = 0;
	f.close();
	
	glShaderSource(shader, 1, (const GLchar**)&source, &length);
	glCompileShader(shader);
	
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	valid = compiled;
	if (!compiled) {
		GLsizei len = 0;
		
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		if (len > 1) {
			GLchar* clog = new GLchar[len];
			glGetShaderInfoLog(shader, len, &len, clog);
			std::cerr << std::endl << "*** unable to compile shader " << path << ":" << std::endl << clog;
			delete clog;
		}
	} else {
		std::cout << "done" << std::endl;
	}
}
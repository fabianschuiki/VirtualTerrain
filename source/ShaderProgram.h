/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Shader.h"


class ShaderProgram
{
public:
	GLuint program;
	bool valid;
	
	ShaderProgram(const char* vert_path, const char* frag_path);
	~ShaderProgram();
	
	void reload();
	void use();
	
	GLint uniform(const char* name);
	
protected:
	Shader vertexShader;
	Shader fragmentShader;
};
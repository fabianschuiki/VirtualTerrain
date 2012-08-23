/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <SFML/OpenGL.hpp>

#include "Planet.h"
#include "SphericalChunk.h"


void SphericalChunk::draw()
{
	vec3 v;
	glBegin(GL_QUADS);
	v = getNormal(0,0); glNormal3f(v.x,v.y,v.z); v = getVertex(0,0); glVertex3f(v.x,v.y,v.z);
	v = getNormal(1,0); glNormal3f(v.x,v.y,v.z); v = getVertex(1,0); glVertex3f(v.x,v.y,v.z);
	v = getNormal(1,1); glNormal3f(v.x,v.y,v.z); v = getVertex(1,1); glVertex3f(v.x,v.y,v.z);
	v = getNormal(0,1); glNormal3f(v.x,v.y,v.z); v = getVertex(0,1); glVertex3f(v.x,v.y,v.z);
	glEnd();
}


vec3 SphericalChunk::getVertex(float x, float y)
{
	return getNormal(x, y) * planet->radius;
}

vec3 SphericalChunk::getNormal(float x, float y)
{
	float p = (p0 + x*(p1-p0)) / 180 * M_PI;
	float t = (t0 + y*(t1-t0)) / 180 * M_PI;
	
	return vec3(cos(t) * sin(p), sin(t), cos(t) * cos(p));
}
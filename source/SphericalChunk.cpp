/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <iostream>
#include <SFML/OpenGL.hpp>

#include "Planet.h"
#include "SphericalChunk.h"


void SphericalChunk::init()
{
	normal = getNormal(0.5, 0.5);
	
	//Upload the vertices to the GPU.
	int num_vertices = (childrenX+1) * (childrenY+1);
	
	struct Vertex {
		float x, y, z;
		float nx, ny, nz;
	} vertices[num_vertices];
	
	for (int y = 0; y <= childrenY; y++) {
		for (int x = 0; x <= childrenX; x++) {
			struct Vertex &vertex = vertices[y*(childrenX+1) + x];
			
			vec3 v = getVertex((float)x / childrenX, (float)y / childrenY);
			vec3 n = getNormal((float)x / childrenX, (float)y / childrenY);
			
			vertex.x = v.x;
			vertex.y = v.y;
			vertex.z = v.z;
			
			vertex.nx = n.x;
			vertex.ny = n.y;
			vertex.nz = n.z;
		}
	}
	
	vertexBuffer.loadData(6*4 * num_vertices, vertices, GL_STATIC_DRAW);
	
	//Update the index buffer.
	indicesResolution = 0;
	updateIndices(2);
}

void SphericalChunk::draw()
{
	if (lod <= 0) return;
	
	vec3 v;
	glColor3f(1.0 - lod, lod, 1 / (p1-p0));
	/*glBegin(GL_QUADS);
	v = getNormal(0,0); glNormal3f(v.x,v.y,v.z); v = getVertex(0,0); glVertex3f(v.x,v.y,v.z);
	v = getNormal(1,0); glNormal3f(v.x,v.y,v.z); v = getVertex(1,0); glVertex3f(v.x,v.y,v.z);
	v = getNormal(1,1); glNormal3f(v.x,v.y,v.z); v = getVertex(1,1); glVertex3f(v.x,v.y,v.z);
	v = getNormal(0,1); glNormal3f(v.x,v.y,v.z); v = getVertex(0,1); glVertex3f(v.x,v.y,v.z);
	glEnd();*/
	
	vertexBuffer.bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 6*4, (void*)0);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 6*4, (void*)12);
	
	indexBuffer.bind();
	glDrawElements(GL_QUADS, num_indices, GL_UNSIGNED_SHORT, (void*)0);
}


void SphericalChunk::updateLOD(vec3 eye, vec3 eye_dir)
{
	vec3 center = getVertex(0.5, 0.5);
	vec3 dir = (eye - center);
	vec3 dir_unit = dir;
	dir_unit.normalize();
	
	//Calculate the angle between the surface normal and the eye vector.
	float angular = normal.dot(dir_unit);
	
	//Calculate the distance between the eye and the chunk center.
	float distance = dir.length() / (getVertex(0,0) - getVertex(1,1)).length();
	
	//std::cout << "d = " << distance << std::endl;
	lod = (angular > -0.25 ? 1 / (1 + distance * 1e-1) : 0);
	
	updateIndices(1.0 / lod);
}


void SphericalChunk::updateIndices(int ir)
{
	if (ir < 1) ir = 1;
	if (ir > childrenX) ir = childrenX;
	if (ir > childrenY) ir = childrenY;
	if (indicesResolution == ir) return;
	indicesResolution = ir;
	
	int nx = childrenX / ir;
	int ny = childrenY / ir;
	
	float dx = (float)childrenX / nx;
	float dy = (float)childrenY / ny;
	
	
	num_indices = 4*nx*ny;
	GLushort indices[num_indices];
	for (int y = 0; y < ny; y++) {
		for (int x = 0; x < nx; x++) {
			int rx0 = x * dx;
			int ry0 = y * dy;
			int rx1 = (x+1) * dx;
			int ry1 = (y+1) * dy;
			if (rx1 > childrenX + 1) rx1 = childrenX + 1;
			if (ry1 > childrenY + 1) ry1 = childrenY + 1;
			
			indices[(y*nx+x)*4 + 0] = ry0 * (childrenX+1) + rx0;
			indices[(y*nx+x)*4 + 1] = ry0 * (childrenX+1) + rx1;
			indices[(y*nx+x)*4 + 2] = ry1 * (childrenX+1) + rx1;
			indices[(y*nx+x)*4 + 3] = ry1 * (childrenX+1) + rx0;
		}
	}
	
	indexBuffer.loadData(2*num_indices, indices, GL_STATIC_DRAW);
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
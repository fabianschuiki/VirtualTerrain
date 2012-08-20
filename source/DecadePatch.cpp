/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cmath>
#include "DecadePatch.h"
#include "Planet.h"


vec3 DecadePatch::getVertex(float x, float y, float z)
{
	float lon = (this->x + 10*x) / 180 * M_PI;
	float lat = (this->y + 10*y) / 180 * M_PI;
	
	return vec3(z * cos(lat) * sin(lon), z * sin(lat), z * cos(lat) * cos(lon));
}

vec3 DecadePatch::getVertexNormal(float x, float y)
{
	return getVertex(x, y, 1);
}


void DecadePatch::updateBuffers()
{
	struct Vertex {
		float x, y, z;
		float nx, ny, nz;
		float s, t;
	} vertices[11*11];
	
	for (int y = 0; y < 11; y++) {
		for (int x = 0; x < 11; x++) {
			float elev00 = planet->elevation.sample(this->x + x, this->y + y);
			float elev01 = planet->elevation.sample(this->x + x, this->y + y + 1);
			float elev10 = planet->elevation.sample(this->x + x + 1, this->y + y);
			
			struct Vertex &v = vertices[y * 11 + x];
			vec3 vv = getVertex(x*0.1, y*0.1, planet->radius + elev00);
			v.x = vv.x;
			v.y = vv.y;
			v.z = vv.z;
			
			vec3 n;
			n.x = -(elev10 - elev00);
			n.y = 1;
			n.z = -(elev01 - elev00);
			n.normalize();
			
			vec3 vn = getVertexNormal(x*0.1, y*0.1);
			/*v.nx = n.y * vn.x + n.x * vn.z - n.z * vn.x * vn.y;
			v.ny = n.y * vn.y + n.z * (vn.x*vn.x + vn.z*vn.z);
			v.nz = n.y * vn.z - n.x * vn.x - n.z * (vn.y * vn.z);*/
			v.nx = vn.x;
			v.ny = vn.y;
			v.nz = vn.z;
			
			v.s = x*0.1;
			v.t = y*0.1;
		}
	}
	
	vertexBuffer.loadData(8 * 4 * 11*11, vertices, GL_STATIC_DRAW);
	
	
	GLushort indices[4*100];
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			indices[(y*10+x)*4 + 0] = y*11+x;
			indices[(y*10+x)*4 + 1] = y*11+x+1;
			indices[(y*10+x)*4 + 2] = (y+1)*11+x+1;
			indices[(y*10+x)*4 + 3] = (y+1)*11+x;
		}
	}
	
	indexBuffer.loadData(2 * 4 * 100, indices, GL_STATIC_DRAW);
}

void DecadePatch::updateNormalMap()
{
	const int dim = 128;
	
	GLbyte n[dim][dim][3];
	
	for (int y = 0; y < dim; y++) {
		for (int x = 0; x < dim; x++) {
			float fx = (float)x / dim;
			float fy = (float)y / dim;
			
			float dx = this->x + fx * 10;
			float dy = this->y + fy * 10;
			
			/*vec3 vn, p;
			p = getVertex(fx, fy, planet->elevation.sample(dx, dy) * 100 + planet->radius);
			
			
			vec3 px = getVertex(fx + 10.0/dim, fy, planet->elevation.sample(dx+1.0/dim,dy) * 100 + planet->radius) - p;
			vec3 py = getVertex(fx, fy + 10.0/dim, planet->elevation.sample(dx,dy+1.0/dim) * 100 + planet->radius) - p;
			vn = px.cross(py);
			vn.normalize();
			
			n[y][x][0] = 255 * (0.5 + vn.x/2);
			n[y][x][1] = 255 * (0.5 + vn.y/2);
			n[y][x][2] = 255 * (0.5 + vn.z/2);*/
			
			float f = planet->elevation.sample(dx,dy) / 4000;
			
			n[y][x][0] = (x == 0 || y == 0 || x == dim-1 || y == dim-1 ? 255 : 0);
			n[y][x][1] = 255 - f*255;
			n[y][x][2] = (f == 0 ? 255 : 0);
		}
	}
	
	normalMap.loadImage(dim, dim, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, n);
}

void DecadePatch::draw()
{
	vertexBuffer.bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 8*4, (void*)0);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 8*4, (void*)12);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 8*4, (void*)24);
	
	indexBuffer.bind();
	normalMap.bind();
	glDrawElements(GL_QUADS, 4*100, GL_UNSIGNED_SHORT, (void*)0);
}
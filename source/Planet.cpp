/*
 * Copyright © 2012 Fabian Schuiki
 */

#include "Planet.h"


Planet::Planet() : radius(6.371e6)
{
	for (int y = -9; y < 9; y++) {
		for (int x = -18; x < 18; x++) {
			SphericalChunk &c = chunks[x+18][y+9];
			c.planet = this;
			c.p0 = (x+0)*10;
			c.p1 = (x+1)*10;
			c.t0 = (y+0)*10;
			c.t1 = (y+1)*10;
			c.childrenX = round(10 * cos((y+0.5) * M_PI / 18));
			c.childrenY = 10;
			c.init();
		}
	}
}

Planet::~Planet()
{
}

void Planet::updateEye(vec3 eye)
{
	vec3 eye_dir = eye;
	eye_dir.normalize();
	
	if ((this->eye - eye).length() < 0.001) return;
	this->eye = eye;
	
	for (int t = 0; t < 18; t++)
		for (int p = 0; p < 36; p++)
			chunks[p][t].updateLOD(eye, eye_dir);
}

void Planet::draw()
{
	for (int t = 0; t < 18; t++)
		for (int p = 0; p < 36; p++)
			chunks[p][t].draw();
}
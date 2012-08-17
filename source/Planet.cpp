/*
 * Copyright © 2012 Fabian Schuiki
 */

#include "Planet.h"


Planet::Planet() : radius(6.371e6)
{
	for (int y = -9; y < 9; y++) {
		for (int x = -18; x < 18; x++) {
			DecadePatch &p = patches[x+18][y+9];
			p.planet = this;
			p.x = x*10;
			p.y = y*10;
			p.normal = p.getVertexNormal(0.5, 0.5);
		}
	}
}

Planet::~Planet()
{
}

void Planet::updateEye(vec3 eye)
{
	eye.normalize();
	if ((this->eye - eye).length() < 0.001) return;
	
	for (int y = 0; y < 18; y++) {
		for (int x = 0; x < 36; x++) {
			DecadePatch &p = patches[x][y];
			p.angularQuality = p.normal.dot(eye);
		}
	}
}
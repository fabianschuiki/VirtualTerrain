/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Vector.h"

class Planet;


class SphericalChunk
{
public:
	Planet *planet;
	
	float p0, p1; //rotation phi (0 = Greenwich)
	float t0, t1; //inclination theta (0 = equator)
	
	bool activeSide[4];
	SphericalChunk *children[4];
	SphericalChunk *parent;
	int level;
	
	void draw();
	
	vec3 getVertex(float x, float y);
	vec3 getNormal(float x, float y);
};
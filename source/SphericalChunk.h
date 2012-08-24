/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Camera.h"
#include "Vector.h"

class Planet;


class SphericalChunk
{
public:
	Planet *planet;
	
	float p0, p1; //rotation phi (0 = Greenwich)
	float t0, t1; //inclination theta (0 = equator)
	
	float pc, tc;
	
	SphericalChunk *parent;
	SphericalChunk *children[4];
	bool activeSides[4];
	int level;
	bool highlighted;
	bool culled;
	
	SphericalChunk();
	~SphericalChunk();
	
	void init();
	void draw();
	void updateDetail(Camera &camera);
	
	vec3 getVertex(float x, float y);
	vec3 getNormal(float x, float y);
	
	void activateChild(int child);
	void deactivateChild(int child);
	SphericalChunk* findAdjacentChunk(int side, bool create = true);
};
/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Chunk.h"


class SphericalChunk : public Chunk
{
public:
	float p0, p1; //rotation phi (0 = Greenwich)
	float t0, t1; //inclination theta (0 = equator)
	
	virtual void init();
	virtual void draw();
	virtual void updateLOD(vec3 eye, vec3 eye_dir);
	
	int indicesResolution;
	int num_indices;
	void updateIndices(int ir);
	
	virtual vec3 getVertex(float x, float y);
	virtual vec3 getNormal(float x, float y);
};
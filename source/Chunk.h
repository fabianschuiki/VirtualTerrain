/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once
#include "Buffer.h"
#include "Vector.h"


class Planet;

class Chunk
{
public:
	Planet *planet;
	
	vec3 normal;
	float lod;
	
	Chunk *children;
	int childrenX;
	int childrenY;
	
	Buffer vertexBuffer;
	Buffer indexBuffer;
	
	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void updateLOD(vec3 eye, vec3 eye_dir) = 0;
	
	virtual vec3 getVertex(float x, float y) = 0;
	virtual vec3 getNormal(float x, float y) = 0;
	
	Chunk();
};
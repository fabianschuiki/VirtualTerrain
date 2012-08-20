/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Patch.h"
#include "Texture.h"
#include "Vector.h"


class Planet;

class DecadePatch : Patch
{
public:
	Planet *planet;
	int x;
	int y;
	vec3 normal;
	float angularQuality;
	Texture normalMap;
	
	vec3 getVertex(float x, float y, float z);
	vec3 getVertexNormal(float x, float y);
	
	void updateBuffers();
	void updateNormalMap();
	void draw();
};
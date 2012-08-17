/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Vector.h"


class Planet;

class DecadePatch
{
public:
	Planet *planet;
	int x;
	int y;
	vec3 normal;
	float angularQuality;
	
	vec3 getVertex(float x, float y, float z);
	vec3 getVertexNormal(float x, float y);
};
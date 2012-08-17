/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cmath>
#include "DecadePatch.h"


vec3 DecadePatch::getVertex(float x, float y, float z)
{
	float lon = (this->x + 10*x) / 180 * M_PI;
	float lat = (this->y + 10*y) / 180 * M_PI;
	
	return vec3(z * cos(lat) * cos(lon), z * sin(lat), z * cos(lat) * sin(lon));
}

vec3 DecadePatch::getVertexNormal(float x, float y)
{
	return getVertex(x, y, 1);
}
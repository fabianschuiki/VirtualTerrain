/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Vector.h"


class Plane
{
public:
	vec3 n;
	float d;
	
	Plane();
	
	void set(vec3 a, vec3 b, vec3 c);
	float getDistance(vec3 p);
};
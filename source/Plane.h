/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Vector.h"


class Plane
{
public:
	vec3 n;
	double d;
	
	Plane();
	
	void set(vec3 a, vec3 b, vec3 c);
	double getDistance(vec3 p);
};
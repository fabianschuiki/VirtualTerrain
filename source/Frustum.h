/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "AABox.h"
#include "Plane.h"
#include "Sphere.h"


class Frustum
{
public:
	typedef enum {
		kOutside   = 0,
		kInside    = 1,
		kIntersect = 2,
	} Coverage;
	
	Plane t, b, l, r, n, f;
	
	Coverage contains(vec3 p);
	Coverage contains(Sphere s);
	Coverage contains(AABox a);
};
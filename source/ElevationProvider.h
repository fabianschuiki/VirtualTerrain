/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Vector.h"


class ElevationProvider
{
public:
	typedef enum Type {
		kOcean,
		kLand,
	};
	
	virtual double getElevation(double x, double y) = 0;
	virtual vec3 getNormal(double x, double y, double r) = 0;
	virtual Type getType(double x, double y) = 0;
};
/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "ElevationProvider.h"


class PerlinElevation : public ElevationProvider
{
public:
	virtual double getElevation(double x, double y);
	virtual vec3 getNormal(double x, double y, double r, double detail);
	virtual Type getType(double x, double y);
};
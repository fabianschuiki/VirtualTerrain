/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "DEMElevationData.h"
#include "ElevationProvider.h"


class DEMElevation : public ElevationProvider
{
public:
	DEMElevationData slices[9][3];
	
	DEMElevation();
	~DEMElevation();
	
	virtual double getElevation(double x, double y, double detail);
	virtual vec3 getNormal(double x, double y, double r, double detail);
	virtual Type getType(double x, double y, double detail);
	
private:
	DEMElevationData* getSlice(double x, double y);
};
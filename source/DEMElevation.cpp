/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cassert>
#include <cmath>
#include "DEMElevation.h"
#include "Planet.h"


DEMElevation::DEMElevation()
{
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 9; x++) {
			DEMElevationData &d = slices[x][y];
			d.container = this;
			d.x = x;
			d.y = y;
		}
	}
}

DEMElevation::~DEMElevation()
{
}


double DEMElevation::getElevation(double x, double y, double detail)
{
	DEMElevationData *data = getSlice(x,y);
	if (!data) return 0;
	return data->getElevation(x, y, detail);
}

vec3 DEMElevation::getNormal(double x, double y, double detail)
{
	/*static const double dx = detail;
	static const double dy = detail / cos(y / 180 * M_PI);
	
	double sx = dx / 180 * M_PI * planet->radius;
	double sy = dy / 180 * M_PI * planet->radius;
	
	double h0 = getElevation(x,y, detail);
	double hx = getElevation(x+dx,y, detail);
	double hy = getElevation(x,y+dy, detail);
	
	vec3 n;
	n.x = sy*(h0-hx);
	n.z = sx*(h0-hy);
	n.y = sx*sy;
	n.normalize();
	return n;*/
	//return vec3(0,1,0);
	
	DEMElevationData *data = getSlice(x,y);
	if (!data) return vec3(0,1,0);
	return data->getNormal(x, y, detail);
}

DEMElevation::Type DEMElevation::getType(double x, double y, double detail)
{
	double e = getElevation(x, y, detail);
	return (e <= 0 ? kOcean : kLand);
}

DEMElevationData* DEMElevation::getSlice(double x, double y)
{
	if (y < -60 || y >= 90) return NULL;
	
	int ix = (x + 180) / 40;
	int iy = (y +  60) / 50;
	
	if (ix >= 9) ix -= 9;
	if (iy >= 3) iy -= 3;
	
	return &slices[ix][iy];
}
/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Vector.h"

class DEMElevation;


class DEMElevationData
{
public:
	DEMElevation *container;
	int x, y;
	
	struct Resolution {
		short w, h;
		int hits;
		short *heights;
		char *normals;
	};
	
	Resolution *resolutions;
	
	DEMElevationData();
	~DEMElevationData();
	
	void load(int resolution);
	void unload(int resolution);
	
	double getElevation(double x, double y, double detail);
	vec3 getNormal(double x, double y, double detail);
	
private:
	Resolution* chooseResolution(double detail);
};
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
		vec3 *normals;
	};
	
	Resolution *resolutions;
	
	DEMElevationData();
	~DEMElevationData();
	
	void load(int resolution);
	void unload(int resolution);
	
	short getElevation(double x, double y, double detail);
};
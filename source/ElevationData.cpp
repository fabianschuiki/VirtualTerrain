/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "ElevationData.h"


ElevationData::ElevationData()
{
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 9; x++) {
			int sx = x * 40 - 180;
			int sy = y * 50 - 10;
			
			char name[256];
			snprintf(name, 256, "/usr/local/share/VirtualTerrain/%c%03i%c%02i.DEM",
					 (sx < 0 ? 'W' : 'E'), abs(sx),
					 (sy < 0 ? 'S' : 'N'), abs(sy));
			
			ElevationDataSlice *s = new ElevationDataSlice(name);
			//s->reload(0);
			slices[x][y] = s;
		}
	}
}

ElevationData::~ElevationData()
{
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 9; x++) {
			delete slices[x][y];
		}
	}
}

float ElevationData::sample(float x, float y)
{
	int sx = (x + 180) / 40;
	int sy = (y + 60) / 50;
	
	sx = sx % 9;
	sy = sy % 3;
	
	float dx = x - (sx * 40 - 180);
	float dy = y - (sy * 50 - 60);
	
	int ix = dx * 4800 / 40;
	int iy = (50-dy) * 6000 / 50;
	
	return slices[sx][sy]->sample(ix, iy);
}
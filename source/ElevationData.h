/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "ElevationDataSlice.h"


class ElevationData
{
public:
	ElevationDataSlice *slices[9][3];
	
	ElevationData();
	~ElevationData();
	
	float sample(float x, float y);
};
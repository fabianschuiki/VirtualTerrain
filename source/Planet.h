/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "SphericalChunk.h"
#include "ElevationData.h"


class Planet
{
public:
	const float radius;
	vec3 eye;
	SphericalChunk chunks[36][18];
	ElevationData elevation;
	
	Planet();
	~Planet();
	
	void updateEye(vec3 eye);
	
	void draw();
};
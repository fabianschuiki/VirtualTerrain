/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "SphericalChunk.h"

class ElevationProvider;


class Planet
{
public:
	const double radius;
	vec3 eye;
	SphericalChunk rootChunk;
	ElevationProvider *elevation;
	
	Planet();
	~Planet();
	
	void updateEye(vec3 eye);
	
	void draw();
};
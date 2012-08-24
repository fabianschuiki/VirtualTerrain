/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Camera.h"
#include "SphericalChunk.h"

class ElevationProvider;


class Planet
{
public:
	const double radius;
	vec3 camera_pos;
	SphericalChunk rootChunk;
	ElevationProvider *elevation;
	
	Planet();
	~Planet();
	
	void updateDetail(Camera &camera);
	
	void draw();
};
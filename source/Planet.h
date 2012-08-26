/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "BakedScenery.h"
#include "Camera.h"
#include "SphericalChunk.h"

class ElevationProvider;


class Planet
{
public:
	const double radius;
	vec3 camera_pos_detail, camera_pos_culling, camera_at;
	SphericalChunk rootChunk;
	ElevationProvider *elevation;
	BakedScenery baked;
	
	Planet();
	~Planet();
	
	void updateDetail(Camera &camera);
	
	void draw();
};
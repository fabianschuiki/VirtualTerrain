/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "BakedScenery.h"
#include "Camera.h"
#include "SphericalChunk.h"
#include <SFML/Graphics/Image.hpp>

class ElevationProvider;


class Planet
{
public:
	const double radius;
	vec3 camera_pos_detail, camera_pos_culling, camera_at;
	SphericalChunk rootChunk;
	ElevationProvider *elevation;
	BakedScenery bakedChunks[12][6];
	sf::Image color;
	
	Planet();
	~Planet();
	
	void updateDetail(Camera &camera);
	
	void draw();
};
/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "DecadePatch.h"
#include "ElevationData.h"


class Planet
{
public:
	const float radius;
	vec3 eye;
	DecadePatch patches[36][18];
	ElevationData elevation;
	
	Planet();
	~Planet();
	
	void updateEye(vec3 eye);
};
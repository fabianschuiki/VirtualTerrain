/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "DecadePatch.h"


class Planet
{
public:
	const float radius;
	vec3 eye;
	DecadePatch patches[36][18];
	
	Planet();
	~Planet();
	
	void updateEye(vec3 eye);
};
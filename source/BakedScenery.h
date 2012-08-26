/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Texture.h"

class Planet;


class BakedScenery
{
public:
	Planet *planet;
	
	double p0, p1;
	double t0, t1;
	
	Texture tex_type; //terrain type
	
	void bake();
};
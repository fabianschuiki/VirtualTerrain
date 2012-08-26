/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include "BakedScenery.h"
#include "Planet.h"


void BakedScenery::bake()
{
	assert(planet);
	std::cout << "baking scenery [" << p0 << "," << p1 << "] x [" << t0 << "," << t1 << "]" << std::endl;
	
	GLubyte *pix_type = new GLubyte[resolution*resolution*4];
	
	for (int y = 0; y < resolution; y++) {
		for (int x = 0; x < resolution; x++) {
			int k = (y * resolution + x) * 4;
			
			double p = p0 + (p1-p0) * (double)x / resolution;
			double t = t0 + (t1-t0) * (double)y / resolution;
			
			ElevationProvider::Type type = planet->elevation->getType(p,t);
			if (type == ElevationProvider::kOcean) {
				pix_type[k+0] = 0;
				pix_type[k+1] = 0.5 * 255;
				pix_type[k+2] = 1 * 255;
				pix_type[k+3] = 255;
			} else {
				pix_type[k+0] = 0;
				pix_type[k+1] = 1 * 255;
				pix_type[k+2] = 0;
				pix_type[k+3] = 255;
			}
		}
	}
	
	tex_type.loadImage(resolution, resolution, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, pix_type);
	
	delete pix_type;
}
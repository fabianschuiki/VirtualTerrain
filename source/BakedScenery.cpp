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
	
	GLubyte *pix_type = new GLubyte[resolution*resolution*3];
	GLubyte *pix_normal = new GLubyte[resolution*resolution*3];
	
	double detail = std::min((p1-p0)/resolution, (t1-t0)/resolution);
	
	for (int y = 0; y < resolution; y++) {
		for (int x = 0; x < resolution; x++) {
			int k = (y * resolution + x) * 3;
			
			double p = p0 + (p1-p0) * (double)x / resolution;
			double t = t0 + (t1-t0) * (double)y / resolution;
			
			/*ElevationProvider::Type type = planet->elevation->getType(p,t);
			if (type == ElevationProvider::kOcean) {
				pix_type[k+0] = 0;
				pix_type[k+1] = 0.5 * 255;
				pix_type[k+2] = 1 * 255;
			} else {
				pix_type[k+0] = 0;
				pix_type[k+1] = 1 * 255;
				pix_type[k+2] = 0;
			}*/
			
			double e = planet->elevation->getElevation(p,t);
			if (e <= 0) {
				pix_type[k+0] = 0;
				pix_type[k+1] = (1 / (1-e/1e3)) * 255;
				pix_type[k+2] = 1 * 255;
			} else {
				pix_type[k+0] = 1 / (1 + e/1000) * 255;
				pix_type[k+1] = (1.0 - e / 8e3 / 2) * 255;
				pix_type[k+2] = 0;
			}
			
			vec3 n = planet->elevation->getNormal(p,t, planet->radius, detail);
			pix_normal[k+0] = 255 * (n.x + 1) / 2;
			pix_normal[k+1] = 255 * (n.y + 1) / 2;
			pix_normal[k+2] = 255 * (n.z + 1) / 2;
		}
	}
	
	tex_type.loadImage(resolution, resolution, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, pix_type);
	tex_normal.loadImage(resolution, resolution, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, pix_normal);
	
	delete pix_type;
}
/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include "BakedScenery.h"
#include "Planet.h"


#define DIM 1024


void BakedScenery::bake()
{
	assert(planet);
	std::cout << "baking scenery [" << p0 << "," << p1 << "] x [" << t0 << "," << t1 << "]" << std::endl;
	
	GLubyte *pix_type = new GLubyte[DIM*DIM*4];
	
	for (int y = 0; y < DIM; y++) {
		for (int x = 0; x < DIM; x++) {
			int k = (y * DIM + x) * 4;
			
			double p = p0 + (p1-p0) * (double)x / DIM;
			double t = t0 + (t1-t0) * (double)y / DIM;
			
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
	
	sf::Image img;
	img.create(DIM, DIM, pix_type);
	img.saveToFile("baked_type.png");
	
	tex_type.loadImage(DIM, DIM, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, pix_type);
	
	delete pix_type;
}
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
	double detail = std::min((p1-p0)/resolution, (t1-t0)/resolution);
	
	assert(planet);
	std::cout << "baking scenery [" << p0 << "," << p1 << "] x [" << t0 << "," << t1 << "] at " << detail << std::endl;
	
	GLubyte *pix_color = new GLubyte[resolution*resolution*3];
	GLubyte *pix_type = new GLubyte[resolution*resolution*3];
	GLubyte *pix_normal = new GLubyte[resolution*resolution*3];
	
	int cw = planet->color.getSize().x;
	int ch = planet->color.getSize().y;
	double fcx = cw / 360.0;
	double fcy = ch / 180.0;
	const sf::Uint8 *cpix = planet->color.getPixelsPtr();
	
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
			
			
			int cx = (int)(( p + 180) * fcx) % cw;
			int cy = (int)((-t +  90) * fcy) % ch;
			int kc = (cy * cw + cx) * 4;
			pix_color[k+0] = cpix[kc+0];
			pix_color[k+1] = cpix[kc+1];
			pix_color[k+2] = cpix[kc+2];
			
			
			double e = planet->elevation->getElevation(p,t, detail);
			if (e <= 0) {
				pix_type[k+0] = 0;
				pix_type[k+1] = (1 / (1-e/1e3)) * 255;
				pix_type[k+2] = 1 * 255;
			} else {
				if (e > 2000) {
					pix_type[k+0] = 255;
					pix_type[k+1] = 255;
					pix_type[k+2] = 255;
				} else {
					pix_type[k+0] = 0;
					pix_type[k+1] = (0.7 + 0.3 / (1 + e/100)) * 255;
					pix_type[k+2] = 0;
				}
			}
			
			vec3 n = planet->elevation->getNormal(p,t, detail);
			pix_normal[k+0] = 255 * (n.x + 1) / 2;
			pix_normal[k+1] = 255 * (n.y + 1) / 2;
			pix_normal[k+2] = 255 * (n.z + 1) / 2;
		}
	}
	
	tex_color.loadImage(resolution, resolution, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, pix_color);
	tex_type.loadImage(resolution, resolution, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, pix_type);
	tex_normal.loadImage(resolution, resolution, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, pix_normal);
	
	delete pix_color;
	delete pix_type;
	delete pix_normal;
}
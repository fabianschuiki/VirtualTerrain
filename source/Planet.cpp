/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <iostream>
#include "Planet.h"
#include "PerlinElevation.h"


Planet::Planet() : radius(6.371e6)
{
	//Initialize some elevation provider.
	elevation = new PerlinElevation;
	
	//Initialize the root chunk which covers the entire planet.
	rootChunk.planet = this;
	rootChunk.p0 = -180;
	rootChunk.p1 = 180;
	rootChunk.t0 = -90;
	rootChunk.t1 = 90;
	rootChunk.init();
}

Planet::~Planet()
{
}

void Planet::updateDetail(Camera &camera)
{
	//Check how much the camera moved or rotated.
	double moved2 = (camera.pos - camera_pos_detail).length2();
	
	//Calculate the size of one pixel on the sphere equator.
	double pixelPerMeter2 = 1 / (camera.pos.length2() - radius*radius) * camera.K * camera.K;
	double meterPerPixel2 = 1 / pixelPerMeter2;
	
	//If the camera moved or rotated a minute amount, update the culling of the terrain.
	if ((camera.pos - camera_pos_culling).length2() > 1e-6 ||
		(camera.at - camera_at).length2() > 1e-6) {
		camera_pos_culling = camera.pos;
		camera_at = camera.at;
		rootChunk.updateCulling(camera);
	}
	
	//Update the terrain if the camera moved an equivalent distance of 100 pixels.
	if (moved2 > meterPerPixel2 * 100 * 100) {
		camera_pos_detail = camera.pos;
		std::cout << "updating detail" << std::endl;
		rootChunk.updateDetail(camera);
	}
}

void Planet::draw()
{
	/*for (int t = 0; t < 18; t++)
		for (int p = 0; p < 36; p++)
			chunks[p][t].draw();*/
	rootChunk.draw();
}
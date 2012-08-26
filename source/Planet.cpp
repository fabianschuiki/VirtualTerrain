/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <iostream>
#include "Planet.h"
#include "DEMElevation.h"


Planet::Planet() : radius(6.371e6)
{
	//Initialize some elevation provider.
	elevation = new DEMElevation;
	elevation->planet = this;
	
	//Initialize the root chunk which covers the entire planet.
	rootChunk.planet = this;
	rootChunk.p0 = -180;
	rootChunk.p1 = 180;
	rootChunk.t0 = -90;
	rootChunk.t1 = 90;
	rootChunk.init();
	
	/*//Initialize the baked scenery chunks.
	for (int y = -3; y < 3; y++) {
		for (int x = -6; x < 6; x++) {
			BakedScenery &b = bakedChunks[x+6][y+3];
			b.planet = this;
			b.p0 = x * 30;
			b.p1 = (x+1) * 30;
			b.t0 = y * 30;
			b.t1 = (y+1) * 30;
			b.resolution = 256;
			b.bake();
		}
	}*/
	
	/*//Initialize some basic form of baked scenery.
	baked.planet = this;
	baked.p0 = -50;
	baked.p1 = 50;
	baked.t0 = -50;
	baked.t1 = 50;
	baked.bake();*/
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
	
	//Update the terrain if the camera moved an equivalent distance of 100 pixels.
	if (moved2 > meterPerPixel2 * 100 * 100) {
		camera_pos_detail = camera.pos;
		std::cout << "updating detail" << std::endl;
		rootChunk.updateDetail(camera);
		
		//Some crappy texture baking.
		/*double delta = 0.5 * (camera.pos.length() / radius - 1) / M_PI * 180;
		if (delta > 90) delta = 90;
		double camp = atan2(camera.pos.x, camera.pos.z) / M_PI * 180;
		double camt = atan(camera.pos.y / sqrt(camera.pos.z*camera.pos.z + camera.pos.x*camera.pos.x)) / M_PI * 180;
		baked.p0 = camp - delta;
		baked.p1 = camp + delta;
		baked.t0 = camt - delta;
		baked.t1 = camt + delta;
		baked.bake();*/
	}
	
	//If the camera moved or rotated a minute amount, update the culling of the terrain.
	if ((camera.pos - camera_pos_culling).length2() > 1e-6 || (camera.at - camera_at).length2() > 1e-6) {
		camera_pos_culling = camera.pos;
		camera_at = camera.at;
		rootChunk.updateCulling(camera);
		rootChunk.updateBakedScenery(camera);
		
		/*std::cout << "visible [" << rootChunk.minp << "," << rootChunk.maxp << "] x [" << rootChunk.mint << "," << rootChunk.maxt << "]" << std::endl;*/
	}
}

void Planet::draw()
{
	/*for (int t = 0; t < 18; t++)
		for (int p = 0; p < 36; p++)
			chunks[p][t].draw();*/
	//bakedChunks[0][0].tex_type.bind();
	rootChunk.draw();
}
/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "DEMElevation.h"
#include "DEMElevationData.h"
#include "Planet.h"

using std::ifstream;
using std::ofstream;


#define NUM_RES 5


DEMElevationData::DEMElevationData()
{
	//Initialize the resolutions.
	resolutions = new Resolution[NUM_RES];
	for (int i = 0; i < NUM_RES; i++) {
		Resolution &r = resolutions[i];
		r.w = 4800 / (1 << i);
		r.h = 6000 / (1 << i);
		r.hits = 0;
		r.heights = NULL;
		r.normals = NULL;
	}
}

DEMElevationData::~DEMElevationData()
{
	//Destroy all resolutions.
	for (int i = 0; i < NUM_RES; i++) {
		Resolution &r = resolutions[i];
		if (r.heights) delete r.heights;
		if (r.normals) delete r.normals;
	}
	delete resolutions;
}

void DEMElevationData::load(int resolution)
{
	//Fetch the target resolution.
	Resolution &r = resolutions[resolution];
	
	//Generate the name of the DEM data file.
	char filename[16];
	int ix = (x * 40 - 180);
	int iy = (y * 50 - 10);
	snprintf(filename, 16, "%c%03i%c%02i.DEM",
			 (ix < 0 ? 'W' : 'E'), abs(ix),
			 (iy < 0 ? 'S' : 'N'), abs(iy));
	
	//Create the cache directory if required.
	const char *cache_dir = "cache";
	mkdir(cache_dir, 0777);
	
	//Create the cache directory for these resolutions if required.
	char resolution_dir[512];
	snprintf(resolution_dir, 512, "%s/DEM%i", cache_dir, resolution);
	mkdir(resolution_dir, 0777);
	
	//Decide where to grab the data from.
	char path[512];
	if (resolution > 0) {
		snprintf(path, 512, "%s/heights_%s", resolution_dir, filename);
	} else {
		snprintf(path, 512, "/usr/local/share/VirtualTerrain/%s", filename);
	}
	
	//Try to load the cache.
	ifstream input(path);
	assert(input.good() || resolution != 0);
	if (input.good()) {
		std::cout << "loading " << path << std::endl;
		
		//Read the heightmap from the file.
		assert(!r.heights);
		r.heights = new short[(int)r.w * r.h];
		input.read((char*)r.heights, (int)r.w * r.h * 2);
		input.close();
		
		//The original DEM files are big endian, so if we read resolution 0 we have to convert.
		if (resolution == 0) {
			std::cout << " - swapping endianness" << std::endl;
			unsigned char *src = (unsigned char *)r.heights;
			for (int i = 0; i < (int)r.w * r.h; i++) {
				unsigned char a = src[i*2];
				src[i*2+0] = src[i*2+1];
				src[i*2+1] = a;
				//r.heights[i] = (r.heights[i] << 8 | r.heights[i] >> 8);
			}
		}
	}
	
	//Downsample the higher resolution cache.
	else if (resolution > 0) {
		//First make sure that the higher resolution cache is loaded.
		Resolution &higher = resolutions[resolution - 1];
		if (!higher.heights)
			load(resolution - 1);
		
		std::cout << "rendering DEM cache {" << x << "," << y << "} at " << resolution << std::endl;
		
		//Downsample the original image.
		//TODO: do some interpolation here!
		assert(!r.heights);
		r.heights = new short[(int)r.w * r.h];
		for (int y = 0; y < r.h; y++) {
			int hy = std::min(y*2, higher.h - 1);
			for (int x = 0; x < r.w; x++) {
				int hx = std::min(x*2, higher.w - 1);
				r.heights[y * r.w + x] = higher.heights[hy * higher.w + hx];
			}
		}
		
		//Store the calculated cache so it may quickly be reloaded the next time.
		ofstream output(path);
		output.write((char*)r.heights, (int)r.w * r.h * 2);
		output.close();
	}
	
	//Make sure that any heights were loaded.
	assert(r.heights);
	
	
	//Decide where the normals are cached.
	char normals_path[512];
	snprintf(normals_path, 512, "%s/normals_%s", resolution_dir, filename);
	
	//Try to load the normals from cache.
	assert(!r.normals);
	r.normals = new char[(int)r.w * r.h * 2];
	ifstream normals_input(normals_path);
	
	//Read the normals if the cache is valid.
	if (normals_input.good()) {
		normals_input.read(r.normals, (int)r.w * r.h * 2);
		normals_input.close();
	}
	
	//Oterhwise calculate the normals.
	else {
		std::cout << "rendering normals cache {" << x << "," << y << "} at " << resolution << std::endl;
		
		for (int y = 0; y < r.h; y++) {
			for (int x = 0; x < r.w; x++) {
				double dx = 40.0 / r.w * cos(((this->y - (double)y / r.h) * 50 - 10) / 180 * M_PI);
				double dy = 50.0 / r.h;
				
				double sx = dx / 180 * M_PI * container->planet->radius * 0.5;
				double sy = dy / 180 * M_PI * container->planet->radius * 0.5;
				
				int xp = std::min(x+1, (int)r.w - 1);
				int yp = std::min(y+1, (int)r.h - 1);
				
				short h0 = r.heights[yp * r.w + x]; //note that yp and y have to be flip since the elevation data extends from top to bottom
				short hx = r.heights[yp * r.w + xp];
				short hy = r.heights[y  * r.w + x];
				
				char *n = &r.normals[(y * r.w + x) * 2];
				double nx = sy * (h0 - hx);
				double nz = sx * (h0 - hy);
				double ny = sx * sy;
				double f = 127.0 / sqrt(nx*nx + ny*ny + nz*nz);
				n[0] = nx * f;
				n[1] = nz * f;
			}
		}
		
		//Store the normals cache.
		ofstream output(normals_path);
		output.write(r.normals, (int)r.w * r.h * 2);
		output.close();
	}
	
	//Make sure that normals were loaded.
	assert(r.normals);
}

void DEMElevationData::unload(int resolution)
{
}

double DEMElevationData::getElevation(double x, double y, double detail)
{
	double fx = fmod((x + 200 + 20) / 40, 1);
	double fy = fmod((y + 100 + 60) / 50, 1);
	Resolution *r = chooseResolution(detail);
	
	double ifx = fx * (r->w - 1);
	double ify = fy * (r->h - 1);
	int ix0 = floor(ifx);
	int ix1 = ceil(ifx);
	int iy0 = floor(ify);
	int iy1 = ceil(ify);
	double dx = ifx - ix0;
	double dy = ify - iy0;
	
	double h00 = r->heights[(r->h-1-iy0)*r->w + ix0];
	double h01 = r->heights[(r->h-1-iy1)*r->w + ix0];
	double h10 = r->heights[(r->h-1-iy0)*r->w + ix1];
	double h11 = r->heights[(r->h-1-iy1)*r->w + ix1];
	
	double h0 = (1-dy)*h00 + dy*h01;
	double h1 = (1-dy)*h10 + dy*h11;
	
	return (1-dx)*h0 + dx*h1;
}

vec3 DEMElevationData::getNormal(double x, double y, double detail)
{
	double fx = fmod((x + 200 + 20) / 40, 1);
	double fy = fmod((y + 100 + 60) / 50, 1);
	Resolution *r = chooseResolution(detail);
	
	double ifx = fx * (r->w - 1);
	double ify = fy * (r->h - 1);
	int ix0 = floor(ifx);
	int ix1 = ceil(ifx);
	int iy0 = floor(ify);
	int iy1 = ceil(ify);
	double dx = ifx - ix0;
	double dy = ify - iy0;
	
	char *n00 = &r->normals[((r->h-1-iy0)*r->w + ix0) * 2];
	char *n01 = &r->normals[((r->h-1-iy1)*r->w + ix0) * 2];
	char *n10 = &r->normals[((r->h-1-iy0)*r->w + ix1) * 2];
	char *n11 = &r->normals[((r->h-1-iy1)*r->w + ix1) * 2];
	
	char n[2];
	for (int i = 0; i < 2; i++) {
		char n0 = n00[i]*(1-dy) + n01[i]*dy;
		char n1 = n10[i]*(1-dy) + n11[i]*dy;
		n[i] = n0*(1-dx) + n1*dx;
	}
	
	double nx = n[0] / 127.0;
	double ny = n[1] / 127.0;
	return vec3(nx, sqrt(1 - nx*nx - ny*ny), ny);
}

DEMElevationData::Resolution* DEMElevationData::chooseResolution(double detail)
{
	
	int res = log2(detail * 120); //since the DEM data is 6000 pixels / 50°
	if (res >= NUM_RES) res = NUM_RES - 1;
	if (res < 0) res = 0;
	
	//Mark a hit at this resolution.
	resolutions[res].hits++;
	
	//Find the highest resolution that has data available.
	for (int i = 0; i <= res; i++)
		if (resolutions[i].heights)
			return &resolutions[i];
	
	//If the no resolution is available, load the appropriate one from the disk.
	load(res);
	return &resolutions[res];
}
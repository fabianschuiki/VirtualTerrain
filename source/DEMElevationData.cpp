/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cassert>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "DEMElevationData.h"

using std::ifstream;
using std::ofstream;


#define NUM_RES 8


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
	
	//Decide where to grab the data from.
	char path[512];
	if (resolution > 0) {
		//Create the cache directory if required.
		const char *cache_dir = "/tmp/VirtualTerrain";
		mkdir(cache_dir, 0777);
		
		//Create the cache directory for these resolutions if required.
		char resolution_dir[512];
		snprintf(resolution_dir, 512, "%s/DEM%i", cache_dir, resolution);
		mkdir(resolution_dir, 0777);
		
		//Assemble the path of the cache for this slice.
		snprintf(path, 512, "%s/%s", resolution_dir, filename);
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
		std::cout << "rendering DEM cache {" << x << "," << y << "} at " << resolution << std::endl;
		
		//First make sure that the higher resolution cache is loaded.
		Resolution &higher = resolutions[resolution - 1];
		if (!higher.heights)
			load(resolution - 1);
		
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
}

void DEMElevationData::unload(int resolution)
{
}

short DEMElevationData::getElevation(double x, double y, double detail)
{
	double fx = fmod((x + 200 + 20) / 40, 1);
	double fy = fmod((y + 100 + 60) / 50, 1);
	
	int res = log2(detail * 120); //since the DEM data is 6000 pixels / 50°
	if (res >= NUM_RES) res = NUM_RES - 1;
	if (res < 0) res = 0;
	
	//Mark a hit at this resolution.
	resolutions[res].hits++;
	
	//Find the closest resolution that has data available.
	Resolution *r = NULL;
	for (int i = res; i >= 0; i--) {
		if (resolutions[i].heights) {
			r = &resolutions[i];
			break;
		}
	}
	
	//If the no resolution is available, load the appropriate one from the disk.
	if (!r) {
		load(res);
		r = &resolutions[res];
	}
	
	int ix = fx * r->w;
	int iy = fy * r->h;
	return r->heights[(r->h - 1 - iy) * r->w + ix];
}

/*short DEMElevationData::sample(int x, int y)
{
	if (!data)
		return 0;
	
	int tx = x >> resolution;
	int ty = y >> resolution;
	
	if (tx < 0) return 0;
	if (tx >= 4800 >> resolution) return 0;
	if (ty < 0) return 0;
	if (ty >= 6000 >> resolution) return 0;
	
	return data[ty * (4800 >> resolution) + tx];
}*/
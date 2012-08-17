/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <iostream>
#include <fstream>
#include "ElevationDataSlice.h"


ElevationDataSlice::ElevationDataSlice(const char *path)
:    path(path)
{
	data = NULL;
	resolution = -1;
}

ElevationDataSlice::~ElevationDataSlice()
{
	unload();
}

void ElevationDataSlice::reload(int resolution)
{
	if (this->resolution == resolution) return;
	unload();
	this->resolution = resolution;
	std::cout << "loading elevation data " << path << " at resolution " << resolution << std::endl;
	
	//Load entire DEM file.
	unsigned int raw_length = (4800 * 6000 * 2);
	char *raw = new char[raw_length];
	std::ifstream f(path);
	if (!f.good()) {
		std::cerr << "*** unable to open elevation data " << path << std::endl;
		return;
	}
	f.read(raw, raw_length);
	f.close();
	
	//Allocate new memory and copy the required samples.
	int tw = 4800 >> resolution;
	int th = 6000 >> resolution;
	data = new short[tw * th];
	
	for (int y = 0; y < th; y++) {
		for (int x = 0; x < tw; x++) {
			int raw_pos = ((y << resolution) * 4800 + (x << resolution)) * 2;
			int data_pos = y * tw + x;
			data[data_pos] = (short)((unsigned char)raw[raw_pos] << 8 | (unsigned char)raw[raw_pos+1]);
		}
	}
	
	//Clean up.
	delete raw;
}

void ElevationDataSlice::unload()
{
	if (data)
		delete data;
	resolution = -1;
}

short ElevationDataSlice::sample(int x, int y)
{
	if (!data)
		return 0;
	
	int tx = x/* >> resolution*/;
	int ty = y/* >> resolution*/;
	
	if (tx < 0) return 0;
	if (tx >= 4800 >> resolution) return 0;
	if (ty < 0) return 0;
	if (ty >= 6000 >> resolution) return 0;
	
	return data[ty * (4800 >> resolution) + tx];
}
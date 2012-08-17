/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once


class ElevationDataSlice
{
public:
	const char * const path;
	short *data;
	int resolution;
	
	ElevationDataSlice(const char *path);
	~ElevationDataSlice();
	
	void reload(int resolution);
	void unload();
	
	short sample(int x, int y);
};
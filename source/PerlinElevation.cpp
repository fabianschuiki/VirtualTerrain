/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cmath>
#include "Perlin.h"
#include "PerlinElevation.h"


static inline double findnoise2(double x, double y)
{
	int n=(int)x+(int)y*57;
	n=(n<<13)^n;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return 1.0-((double)nn/1073741824.0);
}


double PerlinElevation::getElevation(double x, double y)
{
	const static int octaves = 8;
	const static double persistence = 0.5;
	
	double nx = x / 180;
	double ny = y / 180;
	
	double r = 0;
	double n = 0;
	for (int i = 0; i < octaves; i++) {
		double frequency = (1 << i);
		double amplitude = pow(persistence, i);
		
		n += amplitude;
		r += Perlin::noise2(nx * frequency, ny * frequency) * amplitude;
	}
	
	//if (r < 0) r = 0;
	return r * 8e3;
}
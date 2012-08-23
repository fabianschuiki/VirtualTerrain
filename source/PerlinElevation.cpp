/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cmath>
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
	/*return cos(x / 180 * M_PI * 10)*cos(y / 180 * M_PI * 10) * 0.5e6;*/
	
	double result = 1;
	
	const int octaves = 16;
	for (int i = 1; i < octaves+1; i++) {
		int power = (1 << i);
		result += findnoise2(x * power, y * power) / power;
	}
	
	return result * 0.2e6;
}
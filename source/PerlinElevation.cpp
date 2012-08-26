/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <cmath>
#include "Perlin.h"
#include "PerlinElevation.h"


double PerlinElevation::getElevation(double x, double y)
{
	const static int octaves = 10;
	const static double persistence = 0.75;
	
	double nx = x / 180;
	double ny = y / 180;
	
	double r = 0;
	double n = 0;
	for (int i = 0; i < octaves; i++) {
		double frequency = (1 << i) * 2;
		double amplitude = pow(persistence, i);
		
		n += amplitude;
		r += Perlin::noise2(nx * frequency, ny * frequency) * amplitude;
	}
	
	//if (r < 0) r = 0;
	return std::max(r,0.0) * 8e4;
}

vec3 PerlinElevation::getNormal(double x, double y, double r)
{
	static const double dx = 0.01 * cos(y / 180 * M_PI);
	static const double dy = 0.01;
	
	double sx = dx / 180 * M_PI * r;
	double sy = dy / 180 * M_PI * r;
	
	double h0 = getElevation(x,y);
	double hx = getElevation(x+dx,y);
	double hy = getElevation(x,y+dy);
	
	vec3 n;
	n.x = sy*(h0-hx);
	n.z = sx*(h0-hy);
	n.y = sx*sy;
	n.normalize();
	return n;
}

PerlinElevation::Type PerlinElevation::getType(double x, double y)
{
	double e = getElevation(x, y);
	return (e <= 0 ? kOcean : kLand);
}
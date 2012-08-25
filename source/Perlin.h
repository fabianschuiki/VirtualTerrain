/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once


class Perlin
{
public:
	static double noise1(double x);
	static double noise2(double x, double y);
	static double noise3(double x, double y, double z);
};
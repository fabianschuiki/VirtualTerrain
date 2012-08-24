/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once


class Perlin
{
public:
	static float noise1(float x);
	static float noise2(float x, float y);
	static float noise3(float x, float y, float z);
};
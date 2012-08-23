/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once


class ElevationProvider
{
public:
	virtual double getElevation(double x, double y) = 0;
};
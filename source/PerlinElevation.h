/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "ElevationProvider.h"


class PerlinElevation : public ElevationProvider
{
public:
	virtual double getElevation(double x, double y);
};
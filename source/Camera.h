/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Frustum.h"
#include "Vector.h"


class Camera
{
public:
	float fov, aspect;
	float near, far;
	vec3 pos, up, at;
	
	Camera();
	
	void apply();
	Frustum getViewFrustum();
	
private:
};
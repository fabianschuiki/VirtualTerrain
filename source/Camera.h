/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "Frustum.h"
#include "Vector.h"


class Camera
{
public:
	int viewportWidth;
	int viewportHeight;
	
	float fov, aspect;
	float near, far;
	vec3 pos, up, at;
	float K; //perspective scaling factor
	
	Camera();
	
	void apply();
	void applyViewport();
	Frustum getViewFrustum();
	
	void setViewport(int w, int h);
	
private:
};
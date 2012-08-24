/*
 * Copyright © 2012 Fabian Schuiki
 */

#include "Frustum.h"


/** Returns Frustum::kInside if the point p is inside the frustum, Frustum::kOutside otherwise. */
Frustum::Coverage Frustum::contains(vec3 p)
{
	if (t.getDistance(p) < 0) return kOutside;
	if (b.getDistance(p) < 0) return kOutside;
	if (l.getDistance(p) < 0) return kOutside;
	if (r.getDistance(p) < 0) return kOutside;
	if (n.getDistance(p) < 0) return kOutside;
	if (f.getDistance(p) < 0) return kOutside;
	return kInside;
}

/** Returns Frustum::kInside if sphere s is inside the frustum, Frustum::kOutside if it is outside,
 * and Frustum::kIntersect if the sphere intersects the frustum. */
Frustum::Coverage Frustum::contains(Sphere s)
{
	double d;
	Coverage result = kInside;
	
	d = t.getDistance(s.c); if (d < -s.r) return kOutside; else if (d < s.r) result = kIntersect;
	d = b.getDistance(s.c); if (d < -s.r) return kOutside; else if (d < s.r) result = kIntersect;
	d = l.getDistance(s.c); if (d < -s.r) return kOutside; else if (d < s.r) result = kIntersect;
	d = r.getDistance(s.c); if (d < -s.r) return kOutside; else if (d < s.r) result = kIntersect;
	d = n.getDistance(s.c); if (d < -s.r) return kOutside; else if (d < s.r) result = kIntersect;
	d = f.getDistance(s.c); if (d < -s.r) return kOutside; else if (d < s.r) result = kIntersect;
	
	return result;
}
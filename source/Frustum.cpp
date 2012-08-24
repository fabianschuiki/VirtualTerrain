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

/** Returns Frustum::kInside if the box b is entirely inside the frustum, Frustum::kOutside if it is
 * outside and Frustum::kIntersect if the box intersects the frustum. */
Frustum::Coverage Frustum::contains(AABox a)
{
	Plane *planes[] = {&t, &b, &l, &r, &n, &f};
	Coverage result = kInside;
	
	for (int i = 0; i < 6; i++) {
		vec3 &pn = planes[i]->n;
		
		//Calculate the positive and negative vertex of the box.
		vec3 vp(a.x0, a.y0, a.z0);
		if (pn.x >= 0) vp.x = a.x1;
		if (pn.y >= 0) vp.y = a.y1;
		if (pn.z >= 0) vp.z = a.z1;
		
		vec3 vn(a.x1, a.y1, a.z1);
		if (pn.x >= 0) vn.x = a.x0;
		if (pn.y >= 0) vn.y = a.y0;
		if (pn.z >= 0) vn.z = a.z0;
		
		//If the positive vertex is outside, we're entirely outside the frustum.
		if (planes[i]->getDistance(vp) < 0)
			return kOutside;
		
		//If only the negative vertex is outside, we're intersecting the frustum.
		else if (planes[i]->getDistance(vn) < 0)
			result = kIntersect;
	}
	return result;
}
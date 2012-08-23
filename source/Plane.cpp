/*
 * Copyright © 2012 Fabian Schuiki
 */

#include "Plane.h"


Plane::Plane()
{
	d = 0;
}

/** Sets the plane to contain the three points a, b and c. */
void Plane::set(vec3 a, vec3 b, vec3 c)
{
	vec3 ba;
	vec3 bc;
	
	n = bc.cross(ba);
	n.normalize();
	
	d = -n.dot(b);
}

/** Returns the distance of the given point to the plane. */
float Plane::getDistance(vec3 p)
{
	return d + n.dot(p);
}
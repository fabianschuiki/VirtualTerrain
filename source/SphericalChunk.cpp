/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <iostream>
#include <SFML/OpenGL.hpp>

#include "Planet.h"
#include "SphericalChunk.h"


SphericalChunk::SphericalChunk()
{
	for (int i = 0; i < 4; i++) {
		children[i] = NULL;
		activeSides[i] = false;
	}
	
	parent = NULL;
	level = 0;
}

SphericalChunk::~SphericalChunk()
{
	for (int i = 0; i < 4; i++) {
		if (children[i]) delete children[i];
	}
}


void SphericalChunk::init()
{
	//Calculate the phi and theta of the center point.
	pc = (p0+p1)/2;
	tc = (t0+t1)/2;
	
	std::cout << "initialized node [" << p0 << "," << p1 << "] x [" << t0 << "," << t1 << "]" << std::endl;
	
	/*corners[0] = getNormal(1,0);
	corners[1] = getNormal(0,0);
	corners[2] = getNormal(0,1);
	corners[3] = getNormal(1,1);
	
	sides[0] = getNormal(0.5,0);
	sides[1] = getNormal(0,0.5);
	sides[2] = getNormal(0.5,1);
	sides[3] = getNormal(1,0.5);*/
	
	static double thresh = pow(1e6, 2);
	
	if (level < 2) {
		activateChild(0);
		activateChild(1);
		activateChild(2);
		activateChild(3);
	} else {
		//Activate children based on side width for debugging purposes.
		vec3 corners[4] = {getVertex(1,0), getVertex(0,0), getVertex(0,1), getVertex(1,1)};
		for (int i = 0; i < 4; i++) {
			int next = (i < 3 ? i+1 : 0);
			
			double d = (corners[i] - corners[next]).length2();
			//std::cout << " - " << i << ": d = " << d << std::endl;
			if (d > thresh) {
				activateChild(i);
				activateChild(next);
			}
		}
	}
}

void SphericalChunk::draw()
{
	//If not all quadrants are handled by children draw the chunk.
	if (!children[0] || !children[1] || !children[2] || !children[3]) {
		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLE_FAN);
		
		vec3 center   = getVertex(0.5, 0.5);
		vec3 center_n = getNormal(0.5, 0.5);
		glNormal3f(center_n.x, center_n.y, center_n.z);
		glVertex3f(center.x, center.y, center.z);
		
		static const struct { double x; double y; } corners[4] = {{1,1}, {0,1}, {0,0}, {1,0}};
		static const struct { double x; double y; } sides[4] = {{0.5,1}, {0,0.5}, {0.5,0}, {1,0.5}};
		
		//Loop through the quadrants.
		for (int i = 0; i < 4; i++)
		{
			//Add the corner vertex if there is no explicit child for this quadrant.
			if (!children[i]) {
				vec3 corner   = getVertex(corners[i].x, corners[i].y);
				vec3 corner_n = getNormal(corners[i].x, corners[i].y);
				glNormal3f(corner_n.x, corner_n.y, corner_n.z);
				glVertex3f(corner.x, corner.y, corner.z);
			}
			
			//If the side is active, add the corresponding vertex.
			if (activeSides[i]) {
				vec3 side   = getVertex(sides[i].x, sides[i].y);
				vec3 side_n = getVertex(sides[i].x, sides[i].y);
				glNormal3f(side_n.x, side_n.y, side_n.z);
				glVertex3f(side.x, side.y, side.z);
			}
			
			//If the next child exists, move back to the center to avoid drawing over it.
			int next = (i < 3 ? i+1 : 0);
			if (children[next]) {
				glNormal3f(center_n.x, center_n.y, center_n.z);
				glVertex3f(center.x, center.y, center.z);
			}
			
		}
		
		//Finish the last triangle.
		if (!children[0]) {
			vec3 corner   = getVertex(corners[0].x, corners[0].y);
			vec3 corner_n = getNormal(corners[0].x, corners[0].y);
			glNormal3f(corner_n.x, corner_n.y, corner_n.z);
			glVertex3f(corner.x, corner.y, corner.z);
		}
		
		glEnd();
	}
	
	//Draw the children.
	for (int i = 0; i < 4; i++)
		if (children[i]) children[i]->draw();
}


vec3 SphericalChunk::getVertex(float x, float y)
{
	return getNormal(x, y) * planet->radius;
}

vec3 SphericalChunk::getNormal(float x, float y)
{
	float p = (p0 + x*(p1-p0)) / 180 * M_PI;
	float t = (t0 + y*(t1-t0)) / 180 * M_PI;
	
	return vec3(cos(t) * sin(p), sin(t), cos(t) * cos(p));
}


void SphericalChunk::activateChild(int child)
{
	if (children[child] != NULL)
		return;
	
	//Create and initialize the child.
	SphericalChunk *c = new SphericalChunk;
	c->planet = planet;
	c->parent = this;
	c->level  = level + 1;
	
	c->p0 = (child == 1 || child == 2 ? p0 : pc);
	c->p1 = (child == 1 || child == 2 ? pc : p1);
	
	c->t0 = (child == 2 || child == 3 ? t0 : tc);
	c->t1 = (child == 2 || child == 3 ? tc : t1);
	
	children[child] = c;
	c->init();
	
	//Activate the required sides.
	activeSides[child] = true;
	activeSides[child > 0 ? child-1 : 3] = true;
	
	//Activate the adjacent node.
	//...todo...
}
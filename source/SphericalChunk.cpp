/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <iostream>
#include <SFML/OpenGL.hpp>

#include "ElevationProvider.h"
#include "Planet.h"
#include "SphericalChunk.h"


static const struct { double x; double y; } corners[4] = {{1,1}, {0,1}, {0,0}, {1,0}};
static const struct { double x; double y; } sides[4] = {{0.5,1}, {0,0.5}, {0.5,0}, {1,0.5}};


SphericalChunk::SphericalChunk()
{
	for (int i = 0; i < 4; i++) {
		children[i] = NULL;
		activeSides[i] = false;
	}
	
	parent = NULL;
	level = 0;
	highlighted = false;
	culled = false;
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
	
	//std::cout << "initialized node [" << p0 << "," << p1 << "] x [" << t0 << "," << t1 << "]" << std::endl;
	
	/*corners[0] = getNormal(1,0);
	corners[1] = getNormal(0,0);
	corners[2] = getNormal(0,1);
	corners[3] = getNormal(1,1);
	
	sides[0] = getNormal(0.5,0);
	sides[1] = getNormal(0,0.5);
	sides[2] = getNormal(0.5,1);
	sides[3] = getNormal(1,0.5);*/
	
	//static double thresh = pow(1e6, 2);
	
	if (level < 4) {
		activateChild(0);
		activateChild(1);
		activateChild(2);
		activateChild(3);
	}/* else {
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
	}*/
}

void SphericalChunk::draw()
{
	if (culled) return;
	
	//If not all quadrants are handled by children draw the chunk.
	if (!children[0] || !children[1] || !children[2] || !children[3]) {
		bool hl = false;
		for (SphericalChunk *c = this; c; c = c->parent) {
			if (c->highlighted) {
				hl = true;
				break;
			}
		}
		if (hl)
			glColor3f(0, 0.5, 1);
		else
			glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLE_FAN);
		
		vec3 center   = getVertex(0.5, 0.5);
		//vec3 center_n = getNormal(0.5, 0.5);
		vec3 center_n = (getVertex(0,0) - getVertex(1,0)).cross(getVertex(0,0) - getVertex(0,1));
		center_n.normalize();
		glNormal3f(center_n.x, center_n.y, center_n.z);
		glVertex3f(center.x, center.y, center.z);
		
		//Loop through the quadrants.
		for (int i = 0; i < 4; i++)
		{
			//Add the corner vertex if there is no explicit child for this quadrant.
			if (!children[i]) {
				vec3 corner   = getVertex(corners[i].x, corners[i].y);
				vec3 corner_n = getNormal(corners[i].x, corners[i].y);
				glNormal3f(center_n.x, center_n.y, center_n.z);
				glVertex3f(corner.x, corner.y, corner.z);
			}
			
			//If the side is active, add the corresponding vertex.
			if (activeSides[i]) {
				vec3 side   = getVertex(sides[i].x, sides[i].y);
				vec3 side_n = getVertex(sides[i].x, sides[i].y);
				glNormal3f(center_n.x, center_n.y, center_n.z);
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
			glNormal3f(center_n.x, center_n.y, center_n.z);
			glVertex3f(corner.x, corner.y, corner.z);
		}
		
		glEnd();
	}
	
	//Draw the children.
	for (int i = 0; i < 4; i++)
		if (children[i]) children[i]->draw();
}

void SphericalChunk::updateDetail(Camera &camera)
{
	//Calculate the five points of this chunk.
	vec3 corner[4];
	for (int i = 0; i < 4; i++) {
		corner[i] = getVertex(corners[i].x, corners[i].y);
	}
	vec3 center = getVertex(0.5, 0.5);
	
	//If the corners are not in view, don't go into any detail at all.
	Frustum &f = camera.frustum;
	culled = (level > 2 && !f.contains(corner[0]) && !f.contains(corner[1]) && !f.contains(corner[2]) && !f.contains(corner[3]) && !f.contains(center));
	if (culled) return;
	
	//Perform the LOD decision for every child.
	for (int i = 0; i < 4; i++)
	{
		//Calculate the actual and interpolated center of this child.
		vec3 ca = getVertex((corners[i].x + 0.5) / 2, (corners[i].y + 0.5) / 2);
		vec3 ci = (corner[i] + center) / 2;
		
		//Calculate the error in world space.
		double err_world = (ca-ci).length();
		
		//Calculate the error in screen space.
		double D = (ci - camera.pos).length();
		double err_screen = err_world / D * camera.K;
		
		//std::cout << "child " << i << ": err_world = " << err_world << ", err_screen = " << err_screen << std::endl;
		
		//Decide whether the child node is required based on the screen error.
		bool required = err_screen >= 1;
		
		//If the child is required but doesn't exist yet, create one.
		if (required && !children[i]) {
			activateChild(i);
		}
		
		//If the child is not required but does exist, remove it.
		if (!required && children[i]) {
			deactivateChild(i);
		}
	}
	
	//Perform the LOD on the children.
	for (int i = 0; i < 4; i++)
		if (children[i] && level < 16)
			children[i]->updateDetail(camera);
}


vec3 SphericalChunk::getVertex(float x, float y)
{
	return getNormal(x, y) * (planet->radius + planet->elevation->getElevation(p0 + (p1-p0)*x, t0 + (t1-t0)*y));
}

vec3 SphericalChunk::getNormal(float x, float y)
{
	float p = (p0 + x*(p1-p0)) / 180 * M_PI;
	float t = (t0 + y*(t1-t0)) / 180 * M_PI;
	
	return vec3(cos(t) * sin(p), sin(t), cos(t) * cos(p));
}


void SphericalChunk::activateChild(int child)
{
	if (children[child])
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
	int sideA = child;
	int sideB = (child > 0 ? child-1 : 3);
	activeSides[sideA] = true;
	activeSides[sideB] = true;
	
	//Activate the adjacent node's side on side A and side B.
	findAdjacentChunk(sideA)->activeSides[(sideA+2) % 4] = true;
	findAdjacentChunk(sideB)->activeSides[(sideB+2) % 4] = true;
}

void SphericalChunk::deactivateChild(int child)
{
	if (!children[child])
		return;
	
	//Remove the child.
	delete children[child];
	children[child] = NULL;
	
	//Find the adjacent nodes.
	int next = (child < 3 ? child + 1 : 0);
	int prev = (child > 0 ? child - 1 : 3);
	SphericalChunk *adjA = findAdjacentChunk(child, false);
	SphericalChunk *adjB = findAdjacentChunk(prev, false);
	
	//Deactivate adjacent sides if they are not needed anymore.
	if (!children[next]) {
		activeSides[child] = false;
		if (adjA) {
			adjA->deactivateChild((child+2) % 4);
			adjA->deactivateChild((next+2) % 4);
		}
	} else {
		children[next]->deactivateChild(child);
		children[next]->deactivateChild(prev);
		SphericalChunk *adjChildA = (adjA ? adjA->children[prev] : NULL);
		if (adjChildA) {
			adjChildA->deactivateChild((child+2) % 4);
			adjChildA->deactivateChild((next+2) % 4);
		}
	}
	if (!children[prev]) {
		activeSides[prev] = false;
		if (adjB) {
			adjB->deactivateChild((child+2) % 4);
			adjB->deactivateChild((prev+2) % 4);
		}
	} else {
		children[prev]->deactivateChild(child);
		children[prev]->deactivateChild(next);
		SphericalChunk *adjChildB = (adjB ? adjB->children[next] : NULL);
		if (adjChildB) {
			adjChildB->deactivateChild(next);
			adjChildB->deactivateChild((child+2) % 4);
		}
	}
}

SphericalChunk* SphericalChunk::findAdjacentChunk(int side, bool create)
{
	//Calculate the center of the adjacent chunk.
	double w = p1-p0;
	double h = t1-t0;
	double pa = pc, ta = tc;
	if (side == 0) ta += h;
	if (side == 1) pa -= w;
	if (side == 2) ta -= h;
	if (side == 3) pa += w;
	
	//Clamp the center coordinates to a sphere.
	if (pa >  180) pa -= 360;
	if (pa < -180) pa += 360;
	if (ta >  90) ta -= 180;
	if (ta < -90) ta += 180;
	
	//Move up to the root node.
	SphericalChunk *root = NULL;
	for (root = this; root->parent; root = root->parent);
	if (!root) return NULL;
	
	//Perform a dual binary search down to our level.
	SphericalChunk *chunk = root;
	while (chunk->level < level) {
		bool l = pa < chunk->pc;
		bool b = ta < chunk->tc;
		int child = (l ? (b ? 2 : 1) : (b ? 3 : 0));
		
		//Create the child if required.
		if (!chunk->children[child]) {
			if (create)
				chunk->activateChild(child);
			else
				return NULL;
		}
		
		//Step down.
		chunk = chunk->children[child];
	}
	
	return chunk;
}
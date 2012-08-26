/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <iostream>
#include <SFML/OpenGL.hpp>

#include "ElevationProvider.h"
#include "Planet.h"
#include "SphericalChunk.h"

#define MIN_LEVEL 3
#define TAU 1
#define HYSTERESIS 0.5


static const struct { double x; double y; } corner_coeffs[4] = {{1,1}, {0,1}, {0,0}, {1,0}};
static const struct { double x; double y; } side_coeffs[4] = {{0.5,1}, {0,0.5}, {0.5,0}, {1,0.5}};


inline static void glVertex_vec3(vec3 &v) { glVertex3f(v.x, v.y, v.z); }
inline static void glNormal_vec3(vec3 &v) { glNormal3f(v.x, v.y, v.z); }
inline static void drawVertex(SphericalChunk::Vertex &v)
{
	glNormal_vec3(v.unit);
	glColor3f((v.tangent.x + 1) / 2, (v.tangent.y + 1) / 2, (v.tangent.z + 1) / 2);
	glTexCoord2d(v.tex_s, v.tex_t);
	glVertex_vec3(v.position);
}


SphericalChunk::SphericalChunk()
{
	for (int i = 0; i < 4; i++) {
		children[i] = NULL;
		activeSides[i] = false;
	}
	
	parent = NULL;
	level = 0;
	highlighted = false;
	culled_frustum = culled_angle = false;
	baked = NULL;
}

SphericalChunk::~SphericalChunk()
{
	for (int i = 0; i < 4; i++) {
		if (children[i]) delete children[i];
	}
	if (baked) delete baked;
}


void SphericalChunk::init()
{
	//Calculate the phi and theta of the center point.
	pc = (p0+p1)/2;
	tc = (t0+t1)/2;
	detail = std::min((p1-p0)/2, (t1-t0)/2);
	
	//Calculate the spherical unit vectors for each point.
	for (int i = 0; i < 4; i++) {
		double cx = corner_coeffs[i].x;
		double cy = corner_coeffs[i].y;
		double sx = side_coeffs[i].x;
		double sy = side_coeffs[i].y;
		corners[i].unit = getNormal(cx,cy);
		corners[i].tangent = getTangentPhi(cx,cy);
		sides[i].unit = getNormal(sx,sy);
		sides[i].tangent = getTangentPhi(sx,sy);
		updateVertexNormalAndRadius(corners[i], cx, cy);
		updateVertexNormalAndRadius(sides[i], sx, sy);
	}
	center.unit = getNormal(0.5, 0.5);
	center.tangent = getTangentPhi(0.5, 0.5);
	updateVertexNormalAndRadius(center, 0.5, 0.5);
	terrainType = planet->elevation->getType(pc,tc, detail);
	
	//Calculate the bounding box of the chunk.
	boundingBox.x0 = INFINITY;
	boundingBox.y0 = INFINITY;
	boundingBox.z0 = INFINITY;
	boundingBox.x1 = -INFINITY;
	boundingBox.y1 = -INFINITY;
	boundingBox.z1 = -INFINITY;
	for (int i = 0; i < 4; i++) {
		vec3 &v = corners[i].position;
		boundingBox.x0 = std::min<double>(boundingBox.x0, v.x);
		boundingBox.y0 = std::min<double>(boundingBox.y0, v.y);
		boundingBox.z0 = std::min<double>(boundingBox.z0, v.z);
		boundingBox.x1 = std::max<double>(boundingBox.x1, v.x);
		boundingBox.y1 = std::max<double>(boundingBox.y1, v.y);
		boundingBox.z1 = std::max<double>(boundingBox.z1, v.z);
	}
	
	//Activate the children up to the minimum required level of detail.
	if (level <= MIN_LEVEL) {
		activateChild(0);
		activateChild(1);
		activateChild(2);
		activateChild(3);
	}
}

void SphericalChunk::draw()
{
	if (culled_frustum || culled_angle) return;
	
	SphericalChunk *c = this;
	BakedScenery *usedBaking = NULL;
	while (c && !c->baked) c = c->parent;
	if (c) usedBaking = c->baked;
	
	if (usedBaking) {
		glActiveTexture(GL_TEXTURE0);
		usedBaking->tex_type.bind();
		glActiveTexture(GL_TEXTURE1);
		usedBaking->tex_normal.bind();
		glActiveTexture(GL_TEXTURE0);
		
		//Update the vertice's texture coordinates.
		for (int i = 0; i < 4; i++) {
			updateVertexTexture(corners[i], *usedBaking);
			updateVertexTexture(sides[i], *usedBaking);
		}
		updateVertexTexture(center, *usedBaking);
	}
	
	//If not all quadrants are handled by children draw the chunk.
	if (!children[0] || !children[1] || !children[2] || !children[3]) {
		bool hl = false;
		/*for (SphericalChunk *c = this; c; c = c->parent) {
			if (c->highlighted) {
				hl = true;
				break;
			}
		}*/
		if (hl)
			glColor3f(0, 0.5, 1);
		else {
			//glColor3f(1, 1, 1);
			
			/*double H = 90 - 90.0 * (1 / (1 + std::max(0, level - MIN_LEVEL) * 0.5));
			double S = 1;
			double V = 1;
			
			int hi = floor(H/60);
			double f = (H/60 - hi);
			
			double p = V*(1-S);
			double q = V*(1-S*f);
			double t = V*(1-S*(1-f));
			
			switch (hi) {
				case 0:
				case 6: glColor3f(V,t,p); break;
				case 1: glColor3f(q,V,p); break;
				case 2: glColor3f(p,V,t); break;
				case 3: glColor3f(p,q,V); break;
				case 4: glColor3f(t,p,V); break;
				case 5: glColor3f(V,p,q); break;
			}*/
			
			/*switch (terrainType) {
				case ElevationProvider::kOcean: glColor3f(0, 0.25, 0.5); break;
				case ElevationProvider::kLand:  glColor3f(0, 0.5, 0); break;
			}*/
			//BakedScenery &b = planet->baked;
			/*if (pc >= b.p0 && pc <= b.p1 && tc >= b.t0 && tc <= b.t1) {
				glColor3f(0, 1, 0);
			} else {
				glColor3f(1,1,1);
			}*/
			/*if (maxDot > 0.9)
				glColor3f(1,1,1);
			else
			 glColor3f(0.2,0.2,0.2);*/
			glColor3f(1,1,1);
		}
		glBegin(GL_TRIANGLE_FAN);
		
		/*vec3 center   = getVertex(0.5, 0.5);
		//vec3 center_n = getNormal(0.5, 0.5);
		vec3 center_n = (getVertex(0,0) - getVertex(1,0)).cross(getVertex(0,0) - getVertex(0,1));
		center_n.normalize();
		glNormal3f(center_n.x, center_n.y, center_n.z);
		glVertex3f(center.x, center.y, center.z);*/
		
		//Draw the center point.
		drawVertex(center);
		
		//Loop through the quadrants.
		for (int i = 0; i < 4; i++)
		{
			//Add the corner vertex if there is no explicit child for this quadrant.
			if (!children[i]) {
				drawVertex(corners[i]);
			}
			
			//If the side is active, add the corresponding vertex.
			if (activeSides[i]) {
				drawVertex(sides[i]);
			}
			
			//If the next child exists, move back to the center to avoid drawing over it.
			int next = (i < 3 ? i+1 : 0);
			if (children[next]) {
				drawVertex(center);
			}
			
		}
		
		//Finish the last triangle.
		if (!children[0]) {
			drawVertex(corners[0]);
		}
		
		glEnd();
	}
	
	//Draw the children.
	for (int i = 0; i < 4; i++)
		if (children[i]) children[i]->draw();
}

void SphericalChunk::updateDetail(Camera &camera)
{
	if (culled_angle || culled_frustum) return;
	updateCulling(camera);
	
	//Check whether we cover different types of terrain.
	bool differentTypes = false;
	for (int i = 0; i < 4; i++) {
		double x = (corner_coeffs[i].x + 0.5) / 2;
		double y = (corner_coeffs[i].y + 0.5) / 2;
		if (planet->elevation->getType(p0 + (p1-p0)*x, t0 + (t1-t0)*y, detail/2) != terrainType) {
			differentTypes = true;
			break;
		}
	}
	
	//If we cover different types of terrain, check whether the coastline detail is good enough.
	bool increaseCoastline = false;
	if (differentTypes) {
		double distance2 = (center.position - camera.pos).length2();
		double section = (t1-t0) / 180 * M_PI * planet->radius;
		double section2 = section*section;
		double pixels2 = section2 / distance2 * camera.K * camera.K;
		
		//Require a certain amount of coastline detail.
		//increaseCoastline = (pixels2 > 10);
	}
	
	//Perform the LOD decision for every child.
	for (int i = 0; i < 4; i++)
	{
		//Calculate the actual and interpolated center of this child.
		Vertex v;
		double cx = (corner_coeffs[i].x + 0.5) / 2;
		double cy = (corner_coeffs[i].y + 0.5) / 2;
		v.unit = getNormal(cx,cy);
		updateVertexNormalAndRadius(v, cx, cy);
		vec3 ca = v.position;
		vec3 ci = (corners[i].position + center.position) / 2;
		
		//Calculate the error in world space.
		double err_world = (ca-ci).length();
		err_world = std::min(err_world, (t1-t0) / 180 * M_PI * planet->radius / 5 / TAU);
		err_world = std::max(err_world, (t1-t0) / 180 * M_PI * planet->radius / 50 / TAU);
		
		//Calculate the error in screen space.
		double D = (ci - camera.pos).length();
		double err_screen = err_world / D * camera.K;
		
		//std::cout << "child " << i << ": err_world = " << err_world << ", err_screen = " << err_screen << std::endl;
		
		//Decide whether the child node is required based on the screen error.
		bool required = (err_screen > TAU * (1 + HYSTERESIS) || level <= MIN_LEVEL || increaseCoastline);
		bool not_required = (err_screen < TAU / (1 + HYSTERESIS) && !required);
		
		//If the child is required but doesn't exist yet, create one.
		if (required && !children[i]) {
			activateChild(i);
		}
		
		//If the child is not required but does exist, remove it.
		if (not_required && children[i]) {
			SphericalChunk *c = children[i];
			if (!c->children[0] && !c->children[1] && !c->children[2] && !c->children[3])
				deactivateChild(i);
		}
	}
	
	//Calculate the pixel level which is a measure of how many pixels are covered by one side of the vertex.
	double D = (center.position - camera.pos).length();
	pixelLevel = 1e6 / D;
	
	//Perform the LOD on the children.
	for (int i = 0; i < 4; i++)
		if (children[i] && level < 24)
			children[i]->updateDetail(camera);
}

void SphericalChunk::updateBakedScenery(Camera &camera)
{
	double D = (center.position - camera.pos).length();
	double section = (t1-t0) / 180 * M_PI * planet->radius;
	double side = section / D * camera.K;
	double dot = std::max(0.0, (camera.pos - center.position).unit().dot(center.normal));
	dot = dot / 2 + 0.5;
	if (level <= MIN_LEVEL) dot = 1;
	int res = pow(2, floor(log2(side * dot)));
	
	//Bake the scenery if required.
	bool bakeScenery = (level < 24 && !culled_frustum && !culled_angle);
	if (bakeScenery)
		bakeScenery = (level % 2 == 0) && (res >= (baked ? 128 : 256) || level == 0);
	res = 256;
	
	if (!bakeScenery && baked) {
		std::cout << "destroying baked scenery" << std::endl;
		delete baked;
		baked = NULL;
	}
	if (bakeScenery && !baked) {
		std::cout << "creating baked scenery" << std::endl;
		baked = new BakedScenery;
		baked->planet = planet;
		baked->p0 = p0;
		baked->p1 = p1;
		baked->t0 = t0;
		baked->t1 = t1;
		baked->resolution = 0;
	}
	if (bakeScenery && baked) {
		if (res != baked->resolution) {
			std::cout << "baking at " << res << ", side = " << side << ", section = " << section << std::endl;
			baked->resolution = res;
			baked->bake();
		}
	}
	
	//Bake the children's scenery.
	for (int i = 0; i < 4; i++)
		if (children[i])
			children[i]->updateBakedScenery(camera);
}

void SphericalChunk::updateCulling(Camera &camera)
{
	//Check whether our bounding box is inside the frustum.
	Frustum &f = camera.frustum;
	culled_frustum = (level > MIN_LEVEL && f.contains(boundingBox) == Frustum::kOutside);
	
	//Perform the culling on our children as well.
	if (!culled_frustum) {
		for (int i = 0; i < 4; i++)
			if (children[i])
				children[i]->updateCulling(camera);
		
		//Calculate the min and max dot product.
		minDot = 1;
		maxDot = -1;
		
		for (int i = 0; i < 4; i++) {
			vec3 dir = camera.pos - corners[i].position;
			dir.normalize();
			double dot1 = dir.dot(corners[i].unit);
			
			dir = camera.pos - corners[i].position;
			dir.normalize();
			double dot2 = dir.dot(corners[i].normal);
			
			double dot = std::max(dot1, dot2);
			
			if (dot < minDot) minDot = dot;
			if (dot > maxDot) maxDot = dot;
		}
		
		for (int i = 0; i < 4; i++) {
			if (!children[i]) continue;
			double cmin = children[i]->minDot;
			double cmax = children[i]->maxDot;
			if (cmin < minDot) minDot = cmin;
			if (cmax > maxDot) maxDot = cmax;
		}
		
		//Check whether there's any chance of us facing towards the camera.
		culled_angle = (level > MIN_LEVEL && maxDot < 0);
	}
}


/*vec3 SphericalChunk::getVertex(float x, float y)
{
	return getNormal(x, y) * (planet->radius + std::max(0.0, planet->elevation->getElevation(p0 + (p1-p0)*x, t0 + (t1-t0)*y)));
}*/

vec3 SphericalChunk::getNormal(float x, float y)
{
	float p = (p0 + x*(p1-p0)) / 180 * M_PI;
	float t = (t0 + y*(t1-t0)) / 180 * M_PI;
	return vec3(cos(t) * sin(p), sin(t), cos(t) * cos(p));
}

vec3 SphericalChunk::getTangentPhi(float x, float y)
{
	float p = (p0 + x*(p1-p0)) / 180 * M_PI;
	float t = (t0 + y*(t1-t0)) / 180 * M_PI;
	return vec3(cos(t) * cos(p), sin(t), cos(t) * -sin(p));
}

vec3 SphericalChunk::getTangentTheta(float x, float y)
{
	float p = (p0 + x*(p1-p0)) / 180 * M_PI;
	float t = (t0 + y*(t1-t0)) / 180 * M_PI;
	return vec3(-sin(t) * sin(p), cos(t), -sin(t) * cos(p));
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
	//Remove the child.
	if (children[child]) {
		delete children[child];
		children[child] = NULL;
	}
	
	//Find the adjacent nodes.
	int next = (child < 3 ? child + 1 : 0);
	int prev = (child > 0 ? child - 1 : 3);
	SphericalChunk *adjA = findAdjacentChunk(child, false);
	SphericalChunk *adjB = findAdjacentChunk(prev, false);
	
	//Deactivate adjacent sides if they are not needed anymore.
	if (!children[next]) {
		if (activeSides[child]) {
			activeSides[child] = false;
			if (adjA) {
				adjA->deactivateChild((child+2) % 4);
				adjA->deactivateChild((next+2) % 4);
			}
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
		if (activeSides[prev]) {
			activeSides[prev] = false;
			if (adjB) {
				adjB->deactivateChild((child+2) % 4);
				adjB->deactivateChild((prev+2) % 4);
			}
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


/** Updates the given vertex's radius, normal and position based on the data of the planet's ele-
 * vation provider. */
void SphericalChunk::updateVertexNormalAndRadius(Vertex &v, double x, double y)
{
	double p = (p0 + x*(p1-p0));
	double t = (t0 + y*(t1-t0));
	v.p = p;
	v.t = t;
	v.radius = std::max(0.0, planet->elevation->getElevation(p,t, detail)) + planet->radius;
	v.position = v.unit * v.radius;
	
	vec3 n = planet->elevation->getNormal(p, t, planet->radius, 0.01);
	v.normal = v.tangent*n.x + v.unit*n.y + v.unit.cross(v.tangent)*n.z;
}

void SphericalChunk::updateVertexTexture(Vertex &v, BakedScenery &baked)
{
	double s = (v.p - baked.p0) / (baked.p1 - baked.p0);
	double t = (v.t - baked.t0) / (baked.t1 - baked.t0);
	
	if (s < 0) s = 0;
	if (t < 0) t = 0;
	if (s > 1) s = 1;
	if (t > 1) t = 1;
	
	v.tex_s = s;
	v.tex_t = t;
}
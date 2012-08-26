/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include "AABox.h"
#include "BakedScenery.h"
#include "Camera.h"
#include "ElevationProvider.h"
#include "Vector.h"

class Planet;


class SphericalChunk
{
public:
	Planet *planet;
	
	float p0, p1; //rotation phi (0 = Greenwich)
	float t0, t1; //inclination theta (0 = equator)
	
	float pc, tc;
	
	struct Vertex {
		double p,t;
		vec3 unit;
		vec3 tangent;
		vec3 normal;
		vec3 position;
		double tex_s, tex_t;
		double radius;
	};
	Vertex corners[4];
	Vertex sides[4];
	Vertex center;
	
	double pixelLevel;
	
	SphericalChunk *parent;
	SphericalChunk *children[4];
	bool activeSides[4];
	int level;
	bool highlighted;
	
	bool culled_frustum, culled_angle;
	AABox boundingBox;
	double minDot, maxDot;
	
	double minp, mint, maxp, maxt;
	BakedScenery *baked;
	
	ElevationProvider::Type terrainType;
	
	SphericalChunk();
	~SphericalChunk();
	
	void init();
	void draw();
	void updateDetail(Camera &camera);
	void updateCulling(Camera &camera);
	void updateBakedScenery(Camera &camera);
	
	vec3 getVertex(float x, float y);
	vec3 getNormal(float x, float y);
	vec3 getTangentPhi(float x, float y);
	vec3 getTangentTheta(float x, float y);
	
	void activateChild(int child);
	void deactivateChild(int child);
	SphericalChunk* findAdjacentChunk(int side, bool create = true);
	
private:
	void updateVertexNormalAndRadius(Vertex &v, double x, double y);
	void updateVertexTexture(Vertex &v, BakedScenery &baked);
};
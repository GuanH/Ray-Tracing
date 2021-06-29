#pragma once
#include<Windows.h>
#include<cmath>
#include<vector>
#include"structure.h"

class Ray
{
public:
	Ray(float3 pos, float3 dir) :pos(pos), dir(dir) { hop = hop_max; }
private:
	float3 pos;
	float3 dir;
	const int hop_max = 8;
	int hop;
	friend class sphere;
	friend class light;
	friend class plane;
	friend class mesh;
};
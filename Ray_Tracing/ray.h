#pragma once
#include<Windows.h>
#include<cmath>
#include<vector>
#include"structure.h"

class Ray
{
public:
	Ray(rt::float3 pos, rt::float3 dir) :pos(pos), dir(dir) { hop = hop_max; }
	rt::float3 pos;
	rt::float3 dir;
private:
	const int hop_max = 8;
	int hop;
	friend class sphere;
	friend class light;
	friend class plane;
	friend class mesh;
};
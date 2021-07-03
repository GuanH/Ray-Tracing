#pragma once
#include<cmath>
#include"structure.h"

class Camera
{
public:
	Camera(rt::float3 Pos, float r, float p, float y);
	void Update(rt::float3 Pos, float r, float p, float y);
	rt::float3 transformation(rt::float3 p);
	rt::float3 position;
private:
	float roll;
	float pitch;
	float yaw;
	void CalculateMatrix();
	rt::float3 m_1;
	rt::float3 m_2;
	rt::float3 m_3;
};
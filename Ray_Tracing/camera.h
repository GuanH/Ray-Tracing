#pragma once
#include<cmath>
#include"structure.h"

class Camera
{
public:
	Camera(float3 Pos, float r, float p, float y);
	void Update(float3 Pos, float r, float p, float y);
	float3 transformation(float3 p);
	float3 position;
private:
	float roll;
	float pitch;
	float yaw;
	void CalculateMatrix();
	float3 m_1;
	float3 m_2;
	float3 m_3;
};
#include"camera.h"
Camera::Camera(float3 Pos, float r, float p, float y) :position(Pos), roll(r), pitch(p), yaw(y) 
{
	CalculateMatrix();
}

void Camera::Update(float3 Pos, float r, float p, float y)
{
	position = Pos;
	roll = r;
	pitch = p;
	yaw = y;
	CalculateMatrix();
}

float3 Camera::transformation(float3 p)
{
	float x = p.dot(m_1);
	float y = p.dot(m_2);
	float z = p.dot(m_3);
	return float3{ x,y,z };
}

void Camera::CalculateMatrix()
{
	float a = cos(yaw);
	float b = cos(pitch);
	float c = sin(pitch);
	float d = sin(roll);
	float e = sin(yaw);
	float f = cos(roll);
	m_1 = float3{ a * b, a* c* d - e * f, a* c* f + e * d };
	m_2 = float3{ e * b, e * c * d + a * f, e * c * f - a * d };
	m_3 = float3{ -c, b * d, b * f };
}

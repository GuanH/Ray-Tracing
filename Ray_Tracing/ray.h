#pragma once
#include<Windows.h>
#include<cmath>
#include<vector>

struct vec3
{
	vec3() :x(0), y(0), z(0) {};
	vec3(float a, float b, float c) :x(a), y(b), z(c) {};
	float x;
	float y;
	float z;
	inline float dsqr(vec3 r) const
	{
		vec3 dv = *this - r;
		return (dv.x * dv.x + dv.y * dv.y + dv.z * dv.z);
	}
	inline float dot(vec3 r) const
	{
		return x* r.x + y * r.y + z * r.z;
	}
	inline vec3 normalize()
	{
		float n = x * x + y * y + z * z;
		/*fast inverse square root*/
		float x2 = n * 0.5f;
		float y = n;
		long int i = *reinterpret_cast<long int*>(&y);
		i = 0x5f3759df - (i >> 1);
		y = *reinterpret_cast<float*>(&i);
		y *= 1.5f - (x2 * y * y);
		y *= 1.5f - (x2 * y * y);
		/*fast inverse square root*/
		return *this * y;
	}
	inline vec3 saturate(float a,float b)
	{
		return vec3{ (x < a) ? a : ((x > b) ? b : x),
			(y < a) ? a : ((y > b) ? b : y),
			(z < a) ? a : ((z > b) ? b : z) };
	}
	inline vec3 operator+(vec3 r) const
	{
		return vec3{ x + r.x, y + r.y, z + r.z };
	}
	inline vec3 operator-(vec3 r) const
	{
		return vec3{ x - r.x, y - r.y, z - r.z };
	}
	inline vec3 operator*(vec3 r) const
	{
		return vec3{ x * r.x, y * r.y, z * r.z };
	}
	inline void operator+=(vec3 r)
	{
		x += r.x;
		y += r.y;
		z += r.z;
	}
	inline vec3 operator*(float r) const
	{
		return vec3{x*r,y*r,z*r};
	}
	inline vec3 operator/(float r) const
	{
		return vec3{ x / r, y / r, z / r };
	}
	inline bool operator==(vec3 rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}
};

class Ray
{
public:
	Ray(vec3 pos, vec3 dir) :pos(pos), dir(dir) { hop = hop_max; }
private:
	vec3 pos;
	vec3 dir;
	const int hop_max = 8;
	int hop;
	friend class sphere;
	friend class light;
	friend class plane;
};
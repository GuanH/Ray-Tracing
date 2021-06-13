#pragma once
#include<Windows.h>
#include<cmath>
#include<vector>

struct vec3
{
	vec3() :x(0), y(0), z(0) {};
	vec3(double a, double b, double c) :x(a), y(b), z(c) {};
	double x;
	double y;
	double z;
	inline double dsqr(vec3 r) const
	{
		vec3 dv = *this - r;
		return (dv.x * dv.x + dv.y * dv.y + dv.z * dv.z);
	}
	double dot(vec3 r) const
	{
		return x* r.x + y * r.y + z * r.z;
	}
	inline vec3&& normalize()
	{
		double d = sqrt(dsqr(vec3{ 0,0,0 }));
		return *this / d;
	}
	inline vec3&& saturate(double a,double b)
	{
		return vec3{ (x < a) ? a : ((x > b) ? b : x),
			(y < a) ? a : ((y > b) ? b : y),
			(z < a) ? a : ((z > b) ? b : z) };
	}
	vec3 operator+(vec3 r) const
	{
		return vec3{ x + r.x, y + r.y, z + r.z };
	}
	vec3 operator-(vec3 r) const
	{
		return vec3{ x - r.x, y - r.y, z - r.z };
	}
	vec3 operator*(vec3 r) const
	{
		return vec3{ x * r.x, y * r.y, z * r.z };
	}
	void operator+=(vec3 r)
	{
		x += r.x;
		y += r.y;
		z += r.z;
	}
	vec3 operator*(double r) const
	{
		return vec3{x*r,y*r,z*r};
	}
	vec3 operator/(double r) const
	{
		return vec3{ x / r, y / r, z / r };
	}
};

class ray
{
public:
	ray(vec3 pos, vec3 dir) :pos(pos), dir(dir) { hop = hop_max; };
	inline vec3 aim() { return pos + dir * step; }
	inline void forward() { pos = aim(); hop--; }
	inline bool alive() { return hop > 0; }
private:
	vec3 pos;
	vec3 dir;
	const float step = 0.1f;
	const int hop_max = 300;
	int hop;
	friend class sphere;
	friend class light;
	friend class plane;
};
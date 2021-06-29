#pragma once
typedef unsigned char BYTE;

inline float rsqrt(float n)
{
	float x2 = n * 0.5f;
	float y = n;
	long int i = *reinterpret_cast<long int*>(&y);
	i = 0x5f3759df - (i >> 1);
	y = *reinterpret_cast<float*>(&i);
	y *= 1.5f - (x2 * y * y);
	return y*( 1.5f - (x2 * y * y));
}

struct float3
{
	float3() :x(0), y(0), z(0) {};
	float3(float a, float b, float c) :x(a), y(b), z(c) {};
	float x;
	float y;
	float z;
	inline float dsqr(float3 r) const
	{
		float3 dv = *this - r;
		return (dv.x * dv.x + dv.y * dv.y + dv.z * dv.z);
	}
	inline float dot(float3 r) const
	{
		return x * r.x + y * r.y + z * r.z;
	}

	inline float3 cross(float3 r) const
	{
		return float3{ z * r.y - y * r.z,	x * r.z - z * r.x,	y * r.x - x * r.y };
	}

	inline float3 normalize()
	{
		return *this * rsqrt(x * x + y * y + z * z);
	}
	inline float3 saturate(float a, float b)
	{
		return float3{ (x < a) ? a : ((x > b) ? b : x),
			(y < a) ? a : ((y > b) ? b : y),
			(z < a) ? a : ((z > b) ? b : z) };
	}
	inline float3 operator+(float3 r) const
	{
		return float3{ x + r.x, y + r.y, z + r.z };
	}
	inline float3 operator-(float3 r) const
	{
		return float3{ x - r.x, y - r.y, z - r.z };
	}
	inline float3 operator*(float3 r) const
	{
		return float3{ x * r.x, y * r.y, z * r.z };
	}
	inline float3 operator-() const
	{
		return float3{ -x,-y,-z };
	}
	inline void operator+=(float3 r)
	{
		x += r.x;
		y += r.y;
		z += r.z;
	}
	inline float3 operator*(float r) const
	{
		return float3{ x * r,y * r,z * r };
	}
	inline float3 operator/(float r) const
	{
		return float3{ x / r, y / r, z / r };
	}
	inline bool operator==(float3 rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}
};
struct float4
{
	float4() :x(0), y(0), z(0), w(0) {}
	float4(float X, float Y, float Z, float W) :x(X), y(Y), z(Z), w(W) {}
	float4(float X, float3 v) :x(X), y(v.x), z(v.y), w(v.z) {}
	float x;
	float y;
	float z;
	float w;
};
struct Color
{
	BYTE b;
	BYTE g;
	BYTE r;
};

inline float Determinant(float3 a, float3 b, float3 c)
{
	return a.x * b.y * c.z + b.x * c.y * a.z + c.x * a.y * b.z - c.x * b.y * a.z - c.y * b.z * a.x - c.z * b.x * a.y;
}


inline float3 SolveLinear(float3 a, float3 b, float3 c, float3 d)//[ a | b | c ] X = d
{
	float D = Determinant(a, b, c);
	if (D == 0)return{ 0,0,0 };

	float D_A = Determinant(d, b, c);
	float D_B = Determinant(a, d, c);
	float D_C = Determinant(a, b, d);
	float x1 = D_A / D;
	float x2 = D_B / D;
	float x3 = D_C / D;
	return{ x1,x2,x3 };
};
#pragma once
#define _USE_MATH_DEFINES
#include"meshloader.h"
#include"ray.h"
#include"scene.h"
#include<cmath>
#include<vector>
class Scene;
class Mesh;
struct Material
{
	float diffuse = 0;
	float reflectivity = 1;
	float refractive = 0;
	float ambient = 1;
	float3 color = { 1.0f,1.0f,1.0f };
	bool transparent = false;
};

class object
{
public:
	object(float3 position, Scene* s) :pos(position), scene(s) {}
	object(float3 position, Material Mat, Scene* s) :pos(position), material(Mat), scene(s) {}
	Scene* scene;
	float3 pos;
	Material material;
	enum class _Type { _sphere, _light, _plane, _mesh };
	_Type Type = _Type::_sphere;
	virtual float4 Hit(Ray ray, float n) = 0;
	virtual float IsHit(Ray ray) = 0;
	virtual void randomize();
	virtual object* Clone() const = 0;
	virtual void Rotatation(float r, float p, float y) {};
	virtual float3 GetRotation() { return{ 0,0,0 }; };
	bool selected = false;
protected:

	inline float ambient(float s);
	static float ambient_a;//ambient lowest bound
	static float ambient_b;//ambient highest bound
	static float ambient_const;//multiplying by (ambient_const + ambient_b / 2) can map the range (-1.0, 1.0) into (ambient_a, ambient_b)
};

class sphere:public object
{
public:
	sphere(float3 Pos, Scene* s, float r = 1) :object(Pos, s), radius(r), radius2(r * r) { Type = _Type::_sphere; }
	sphere(float3 Pos, Material Mat, Scene* s, float r = 1) :object(Pos, Mat, s), radius(r), radius2(r* r){ Type = _Type::_sphere; }
	float4 Hit(Ray ray, float n);
	void randomize();
	object* Clone() const
	{
		return new sphere({ 0,0,7 }, material, scene, radius);
	};

	inline float IsHit(Ray ray)
	{
		float b = 2.0f * (ray.dir.dot(ray.pos - pos));
		float c = ray.pos.dot(ray.pos) + pos.dot(pos) - 2.0f * ray.pos.dot(pos) - radius2;
		float d = b * b - 4.0f * c;
		//the sphere is not Hit
		if (d < 0.00001)
		{
			return -1;
		}
		return (-b - sqrtf(d)) / 2.0f;
	};
private:
	float radius = 1;
	float radius2 = 1;
};

class light:public object
{
public:
	light(float3 Pos, Scene* s) :object(Pos, s) { Type = _Type::_light; material.transparent = true; }
	light(float3 Pos, float l, Scene* s, float r = 1) :object(Pos, s), luminance(l), radius(r), radius2(r * r) { Type = _Type::_light; material.transparent = true;}
	light(float3 Pos, Material Mat, float l, Scene* s, float r = 1) :object(Pos,Mat,s), luminance(l), radius(r), radius2(r * r){ Type = _Type::_light; material.transparent = true;}
	float4 Hit(Ray ray, float n);
	void randomize();
	object* Clone() const
	{
		return new light({ 0,0,7 }, material, luminance, scene, radius);
	}
	inline float IsHit(Ray ray)
	{
		float b = 2.0f * (ray.dir.dot(ray.pos - pos));
		float c = ray.pos.dot(ray.pos) + pos.dot(pos) - 2.0f * ray.pos.dot(pos) - radius2;
		float d = b * b - 4.0f * c;
		if (d < 0.00001)
		{
			return -1;
		}
		return (-b - sqrtf(d)) / 2.0f;
	};
private:
	float radius = 1;
	float radius2 = 1;
	float luminance = 15;
};

class plane :public object
{
public:
	plane(float3 Pos, Scene* s) :object(Pos, s) { Type = _Type::_plane; };
	plane(float3 Pos,Material Mat, Scene* s) :object(Pos, Mat, s){ Type = _Type::_plane; };
	float4 Hit(Ray ray, float n);
	void randomize();
	object* Clone() const
	{
		return new plane({ 0,0,7 }, material, scene);
	}
	inline float IsHit(Ray ray)
	{
		return (pos.y - ray.pos.y) / ray.dir.y;
	}
};

class mesh :public object
{
public:
	mesh(Mesh* M, float3 Position, Material Mat, Scene* scene);
	float4 Hit(Ray ray, float n);
	float3 Refract(Ray ray, float3 hit_pos, float3 normal, int hit_index);
	void randomize();
	float IsHit(Ray ray);
	void Rotatation(float r, float p, float y);
	float3 GetRotation() { return{ roll,pitch,yaw }; };
	object* Clone() const
	{
		return new mesh(m, { 0,0,7 }, material, scene);
	}
	struct Mesh_Hit_Type
	{
		float t;
		int hit_index;
	};

	inline Mesh_Hit_Type _IsHit(Ray ray);


	inline float IsHitTriangle(Ray ray, float3 a, float3 b, float3 c, float n)
	{
		float3 A = b - a;
		float3 B = c - a;
		float3 uvt = SolveLinear(A, B, -ray.dir, ray.pos - a);
		float u = uvt.x;
		float v = uvt.y;
		float t = uvt.z;

		if (u > -0.00001 && v > -0.00001 && u + v <= 1 && (t < n || n < 0))
		{
			return t;
		}
		return -1;
	};
private:
	Mesh* m;
	float roll = 0;
	float pitch = 0;
	float yaw = 0;
	std::vector<float3> vertices;
};
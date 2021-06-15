#pragma once
#include"scene.h"
#include"ray.h"
#include<cmath>
#include<tuple>
class Scene;
struct Color;
class object
{
public:
	object(vec3 position, Scene* s) :pos(position), scene(s) {}
	object(vec3 position,vec3 c, Scene* s) :pos(position), color(c), scene(s) {}
	Scene* scene;
	void SetPosition(vec3 p) { pos = p; }
	void SetColor(vec3 p) { color = p; }
	virtual std::pair<float, vec3> reflect(Ray r) = 0;
protected:
	vec3 pos;
	vec3 color = { 1.0f, 1.0f, 1.0f };
	inline float ambient(float s);
	static float ambient_a;//ambient lowest bound
	static float ambient_b;//ambient highest bound
	static float ambient_const;//multiplying by (ambient_const + ambient_b / 2) can map the range (-1.0, 1.0) into (ambient_a, ambient_b)
};

class sphere:public object
{
public:
	sphere(vec3 position, Scene* s) :object(position, s) {}
	sphere(float x, float y, float z, Scene* s, float r = 1) :object(vec3{ x, y, z }, s), radius(r), radius2(r * r) {}
	sphere(float x, float y, float z, float a, float b, float c, Scene* s, float r = 1) :object(vec3{ x, y, z }, vec3{ a, b, c }, s), radius(r), radius2(r * r) {}
	std::pair<float, vec3> reflect(Ray ray);
private:
	float radius = 1;
	float radius2 = 1;
};

class light:public object
{
public:
	light(vec3 position, Scene* s) :object(position, s) {}
	light(float x, float y, float z, float l, Scene* s, float r = 1) :object(vec3{ x, y, z }, s), luminance(l), radius(r), radius2(r * r) {}
	light(float x, float y, float z, float a, float b, float c, float l, Scene* s, float r = 1) :object(vec3{ x, y, z },vec3{ a, b, c },s), luminance(l), radius(r), radius2(r * r){}
	std::pair<float, vec3> reflect(Ray ray);
private:
	float radius = 1;
	float radius2 = 1;
	float luminance = 15;
};

class plane :public object
{
public:
	plane(vec3 position, Scene* s) :object(position, s) {};
	plane(float x, float y, float z, Scene* s) :object(vec3{ x,y,z }, s) {};
	plane(float x, float y, float z, float a, float b, float c, Scene* s) :object(vec3{ x, y, z }, vec3{ a, b, c }, s) {};
	std::pair<float, vec3> reflect(Ray ray);
};
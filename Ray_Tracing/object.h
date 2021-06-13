#pragma once
#include"scene.h"
#include"ray.h"
#include<cmath>
class Scene;
struct color;
class object
{
public:
	object(vec3 position, Scene* s) :pos(position), scene(s) {};
	Scene* scene;
	virtual bool hit(ray& r) = 0;
	virtual vec3 reflect(ray& r) = 0;
protected:
	vec3 pos;
	static double ambient_a;//ambient lowest bound
	static double ambient_b;//ambient highest bound
	static double ambient_const;//multiplying by (ambient_const + ambient_b / 2) can map the range (-1.0, 1.0) into (ambient_a, ambient_b)
};

class sphere:public object
{
public:
	sphere(vec3 position, Scene* s) :object(position, s) {};
	sphere(double x, double y, double z, Scene* s) :object(vec3{ x, y, z }, s) {};
	sphere(double x, double y, double z, double a, double b, double c, Scene* s) :object(vec3{ x, y, z }, s), color(vec3{ a, b, c }) {};
	bool hit(ray& r);
	vec3 reflect(ray& r);
private:
	double radius = 1;
	vec3 color{1.0, 1.0, 1.0};
};

class light:public object
{
public:
	light(vec3 position, Scene* s) :object(position, s) {};
	light(double x, double y, double z, double l, Scene* s, double r = 1) :object(vec3{ x, y, z }, s), luminance(l), radius(r) {};
	light(double x, double y, double z, double a, double b, double c, double l, Scene* s, double r = 1) :object(vec3{ x, y, z }, s), color(vec3{ a, b, c }), luminance(l), radius(r) {};
	bool hit(ray& r);
	vec3 reflect(ray& r);
private:
	double radius = 1;
	double luminance = 15;
	vec3 color{ 1.0, 1.0, 1.0 };
};

class plane :public object
{
public:
	plane(vec3 position, Scene* s) :object(position, s) {};
	plane(double x, double y, double z, Scene* s) :object(vec3{ x,y,z }, s) {};
	plane(double x, double y, double z, double a, double b, double c, Scene* s) :object(vec3{ x, y, z }, s), color(vec3{ a, b, c }) {};
	bool hit(ray& r);
	vec3 reflect(ray& r);
private:
	vec3 color{ 0.2, 0.2, 0.2 };
};
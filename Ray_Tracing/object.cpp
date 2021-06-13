#include "object.h"
/*static member declare*/
double object::ambient_a = 0;
double object::ambient_b = 0.7;
double object::ambient_const = (object::ambient_b - object::ambient_a) / 2.0;
/*static member declare*/

bool sphere::hit(ray& r)
{
	return (pos.dsqr(r.aim()) < radius * radius);
}

vec3 sphere::reflect(ray& ray)
{
	double t = (pos - ray.pos).dot(ray.dir);
	vec3 mid = ray.dir * t + ray.pos;
	vec3 chord2center = mid - pos;
	vec3 hit_pos = ray.dir * (t - sqrt(radius * radius - chord2center.dot(chord2center))) + ray.pos;
	vec3 normal = (hit_pos - pos).normalize();
	ray.dir = (normal * (normal.dot(ray.pos - hit_pos)) * 2 + hit_pos - ray.pos).normalize();
	ray.pos = hit_pos;
	
	while (ray.alive())
	{
		for (auto o : scene->objs)
		{
			if (o->hit(ray))
			{
				vec3 light = o->reflect(ray) * 0.8;//luminance decreases every time the ray hits the sphere
				double s = normal.dot(vec3{ 0, 1, 0 });
				return light + color * (s * ambient_const + ambient_b / 2.0);
			}
		}
		ray.forward();
	}
	double s = normal.dot(vec3{0, 1, 0});
	return color * (s * ambient_const + ambient_b / 2.0);
}

bool light::hit(ray& r)
{
	return (pos.dsqr(r.aim()) < radius * radius);
}

vec3 light::reflect(ray& r)
{
	float d = (r.hop_max - r.hop) * r.step;
	return color * (luminance / (d * d));
}

bool plane::hit(ray& r)
{
	vec3 a = r.aim();
	return (r.pos.y > pos.y && pos.y > a.y) || (r.pos.y < pos.y && pos.y < a.y);
}

vec3 plane::reflect(ray& r)
{
	r.pos = r.pos + r.dir * ((pos.y - r.pos.y) / r.dir.y);
	r.dir.y = -r.dir.y;
	while (r.alive())
	{
		for (auto o : scene->objs)
		{
			if (o->hit(r))
			{
				vec3 light = o->reflect(r) * 0.5;//luminance decreases every time the ray hits the plane
				return light + color * (ambient_const + ambient_b / 2);
			}
		}
		r.forward();
	}
	return std::move(color);
}
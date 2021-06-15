#include "object.h"
/*static member declare*/
float object::ambient_a = 0.0f;
float object::ambient_b = 0.7f;
float object::ambient_const = (object::ambient_b - object::ambient_a) / 2.0f;
/*static member declare*/

//return { distance from the camera, color }
std::pair<float, vec3> sphere::reflect(Ray ray)
{
	float b = 2.0f * (ray.dir.dot(ray.pos-pos));
	float c = ray.pos.dot(ray.pos) + pos.dot(pos) - 2.0f * ray.pos.dot(pos) - radius2;
	float d = b * b - 4.0f * c;
	//the sphere is not hit
	if (d < 0)
	{
		return { -1, vec3{0,0,0}};
	}
	float t = (-b - std::sqrt(d)) / 2.0f;
	vec3 hit_pos = ray.pos + ray.dir * t;
	//opposite direction or it hit the same point due to truncation error
	if (t < 0 || hit_pos == ray.pos)
	{
		return { -1, vec3{0,0,0} };
	}
	vec3 normal = (hit_pos - pos).normalize();
	ray.dir = (normal * (normal.dot(ray.pos - hit_pos)) * 2.0f + hit_pos - ray.pos).normalize();
	ray.pos = hit_pos;
	vec3 ambient_light = color * ambient(normal.dot(vec3{ 0, 1, 0 }));
	if (--ray.hop < 0)
	{
		return { t , ambient_light };
	}
	float nearest = -1;
	vec3 Color = { 0,0,0 };
	for (auto o : scene->objs)
	{
		std::pair<float, vec3> light = o->reflect(ray);
		if (light.first > 0)
		{
			if (light.first < nearest || nearest < 0)
			{
				nearest = light.first;
				Color = light.second;
			}
		}
	}
	float s = normal.dot(vec3{ 0, 1, 0 });
	return { t, Color * 0.7f + ambient_light };
}

std::pair<float, vec3> light::reflect(Ray ray)
{
	float b = 2.0f * (ray.dir.dot(ray.pos - pos));
	float c = ray.pos.dot(ray.pos) + pos.dot(pos) - 2.0f * ray.pos.dot(pos) - radius2;
	float d = b * b - 4.0f * c;
	if (d < 0)
	{
		return { -1, vec3{0,0,0} };
	}
	float t = (-b - std::sqrt(d)) / 2.0f;
	float m = ray.dir.dot(pos - ray.pos) - t;
	float x =  m / radius;
	return { t, scene->sky + color * luminance * x / t / t };
}

std::pair<float, vec3> plane::reflect(Ray ray)
{
	float t = (pos.y - ray.pos.y) / ray.dir.y;
	if (t < 0 || (ray.hop < 0) || (ray.dir.y == 0))
	{
		return { -1, vec3{0,0,0} };
	}
	vec3 ambient_light = color * ambient(1);
	if (ray.hop < 0)
	{
		return { t,ambient_light };
	}
	ray.hop--;
	ray.pos = ray.pos + ray.dir * t;
	ray.dir.y = -ray.dir.y;
	float nearest = -1;
	vec3 Color = color;
	for (auto o : scene->objs)
	{
		std::pair<float, vec3> light = o->reflect(ray);
		if (light.first > 0)
		{
			if (light.first < nearest || nearest < 0)
			{
				nearest = light.first;
				Color = light.second;
			}
		}
	}
	return { t, Color * 0.2f + ambient_light };
}

inline float object::ambient(float s)
{
	return s * ambient_const + ambient_b / 2.0f;
}
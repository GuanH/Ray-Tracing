#include "scene.h"

Scene::Scene(int& resolutionx, int& resolutiony) :resolutionx(resolutionx), resolutiony(resolutiony)
{
	objs =
	{
		new sphere(1.5,  0.0,  2.0, this) ,
		new sphere(-1.5,  0.0,  3.0,  1.0,  0.0,  0.0, this),
		new sphere(-0.3,  0.8,  4.0,  0.0,  0.0,  1.0, this),
		new sphere(1.0,  -1.0,  3.5,  1.0,  1.0,  0.0, this),
		new light(0.0,  4.0,  0.0,  40, this),
		new plane(0.0,  -2.0,  0.0, this)
	};
}

color&& Scene::tracepixel(int x, int y)
{
	/*projection plane*/
	constexpr double w = 1.0;
	constexpr double h = 1.0;
	constexpr double d = 1.0;
	/*projection plane*/
	double pointingx = w * ( 2.0 * x / resolutionx -1.0);
	double pointingy = h * ( 2.0 * y / resolutiony -1.0);
	vec3 dir{ pointingx, pointingy, d };
	ray ray(vec3{ 0, 0, 0 }, dir.normalize());

	while (ray.alive())
	{
		for (auto o : objs)
		{
			if (o->hit(ray))
			{
				return translate_color((o->reflect(ray)).saturate(0.0, 1.0));
			}
		}
		ray.forward();
	}
	return color{ 234, 124, 72 };//sky blue
}

color&& Scene::translate_color(vec3 c)
{
	return color{ static_cast<BYTE>(c.z * 255.0) ,static_cast<BYTE>(c.y * 255.0) ,static_cast<BYTE>(c.x * 255.0) };
}
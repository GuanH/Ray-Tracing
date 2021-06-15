#include "scene.h"

Scene::Scene(int& resolutionx, int& resolutiony) :resolutionx(resolutionx), resolutiony(resolutiony)
{
	objs =
	{
		new light(-2.0f,  1.0f,  2.0f,  3.0f, this, 0.25f),
		new plane(0.0f,  -3.0f,  0.0f,  0.2f,  0.2f,  0.2f,  this),
		new sphere(1.5f,  0.0f,  2.0f, this) ,
		new sphere(-1.5f,  0.0f,  3.0f,  1.0f,  0.0f,  0.0f, this),
		new sphere(-0.3f,  0.8f,  4.0f,  0.0f,  0.0f,  1.0f, this),
		new sphere(1.0f,  -1.0f,  3.5f,  1.0f,  1.0f,  0.0f, this),
	};
}

Color&& Scene::tracepixel(int x, int y)
{
	float pointingx = w * ( 2.0f * x / resolutionx -1.0f);
	float pointingy = h * ( 1.0f - 2.0f * y / resolutiony );
	vec3 dir{ pointingx, pointingy, d };
	Ray ray(vec3{ 0, 0, 0 }, dir.normalize());

	float nearest = -1;
	vec3 color = sky;
	for (auto o : objs)
	{
		std::pair<float, vec3> light = o->reflect(ray);
		if (light.first > 0)
		{
			if (light.first < nearest || nearest < 0)
			{
				nearest = light.first;
				color = light.second.saturate(0.0f, 1.0f);
			}
		}
	}
	return translate_color(color);
}

Color&& Scene::translate_color(vec3 c)
{
	return Color{ static_cast<BYTE>(c.z * 255.0f) ,static_cast<BYTE>(c.y * 255.0f) ,static_cast<BYTE>(c.x * 255.0f) };
}
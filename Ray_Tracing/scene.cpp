#include "scene.h"

Scene::Scene(int& resolutionx, int& resolutiony) :resolutionx(resolutionx), resolutiony(resolutiony)
{
	Material m_plane;
	m_plane.color = { 0.498f,0.576f,0.674f };
	m_plane.diffuse = 0;
	m_plane.reflectivity = 0.8f;

	Material m_sphere;
	m_sphere.color = { 1,0,0 };
	m_sphere.diffuse = 0;
	m_sphere.reflectivity = 0.8f;
	m_sphere.transparent = false;
	m_sphere.refractive = 1.5f;
	objs.add(new light(float3{ -1000,  1000,  -1000 }, 100.0f, this, 100));
	objs.add(new plane(float3{ 0.0f,  -5.0f,  0.0f }, m_plane, this));
	objs.add(new sphere(float3{ -2, -4,  7.0f }, m_sphere, this, 1.0f));

	objs.SetRandomizeLock(0, true);
	objs.SetRandomizeLock(1, true);
	objs.SetRandomizeLock(2, true);
}

Color Scene::tracepixel(int x, int y)
{
	float pointingx = w * (static_cast<float>(x) / resolutionx - 0.5f);
	float pointingy = h * (0.5f - static_cast<float>(y) / resolutiony);

	float3 dir{ pointingx, pointingy, d };
	dir = camera.transformation(dir);
	Ray ray{ camera.position, dir.normalize() };
	float nearest = -1;
	float3 color = sky;

	for (int i = 0; i < objs.size(); i++)
	{
		float4 light = objs[i]->Hit(ray, nearest);
		if (light.x > 0.00001)
		{
			if (objs[i]->selected)
			{
				float test_x;
				float test_y;
				{
					test_x = static_cast<float>(x) + 1.0f;
					test_y = static_cast<float>(y);
					pointingx = w * (test_x / resolutionx - 0.5f);
					pointingy = h * (0.5f - test_y / resolutiony);
					dir = camera.transformation(float3{ pointingx, pointingy, d });
					Ray test_ray{ camera.position, dir.normalize() };
					if (objs[i]->IsHit(test_ray) < 0.00001)
					{
						return{ 255,255,255 };
					}
				}
				{
					test_x = static_cast<float>(x) - 1.0f;
					test_y = static_cast<float>(y);
					pointingx = w * (test_x / resolutionx - 0.5f);
					pointingy = h * (0.5f - test_y / resolutiony);
					dir = camera.transformation(float3{ pointingx, pointingy, d });
					Ray test_ray{ camera.position, dir.normalize() };
					if (objs[i]->IsHit(test_ray) < 0.00001)
					{
						return{ 255,255,255 };
					}
				}
				{
					test_x = static_cast<float>(x);
					test_y = static_cast<float>(y) + 1.0f;
					pointingx = w * (test_x / resolutionx - 0.5f);
					pointingy = h * (0.5f - test_y / resolutiony);
					dir = camera.transformation(float3{ pointingx, pointingy, d });
					Ray test_ray{ camera.position, dir.normalize() };
					if (objs[i]->IsHit(test_ray) < 0.00001)
					{
						return{ 255,255,255 };
					}
				}
				{
					test_x = static_cast<float>(x);
					test_y = static_cast<float>(y) - 1.0f;
					pointingx = w * (test_x / resolutionx - 0.5f);
					pointingy = h * (0.5f - test_y / resolutiony);
					dir = camera.transformation(float3{ pointingx, pointingy, d });
					Ray test_ray{ camera.position, dir.normalize() };
					if (objs[i]->IsHit(test_ray) < 0.00001)
					{
						return{ 255,255,255 };
					}
				}
			}


			if (light.x < nearest || nearest < 0)
			{
				nearest = light.x;
				color = float3{ light.y,light.z,light.w }.saturate(0.0f, 1.0f);
			}
		}
	}
	return translate_color(color);
}

Color Scene::translate_color(float3 c)
{
	return Color{ static_cast<BYTE>(c.z * 255.0f) ,static_cast<BYTE>(c.y * 255.0f) ,static_cast<BYTE>(c.x * 255.0f) };
}

size_t Scene::Select(float x, float y)
{
	float pointingx = w * x;
	float pointingy = h * y;
	float3 dir{ pointingx, pointingy, d };
	dir = camera.transformation(dir);
	Ray ray{ camera.position, dir.normalize() };

	float nearest = -1;
	size_t index = 0;
	for (int i = 0; i < objs.size(); i++)
	{
		float t = objs[i]->IsHit(ray);
		if ((t > 0.00001) && (t < nearest || nearest < 0))
		{
			nearest = t;
			index = i;
		}
	}

	if (selected != -1)
	{
		objs[selected]->selected = false;
	}

	if (nearest > 0)
	{
		objs[index]->selected = true;
		return selected = index;
	}
	else
	{
		return selected = -1;
	}
}

void Scene::AddMesh(std::string path)
{
	Mesh* m = new Mesh(path);
	Material material;
	material.color = { 1,1,1 };
	material.diffuse = 0;
	material.reflectivity = 0.4f;
	material.transparent = false;
	material.refractive = 1.5;
	objs.add(new mesh(m, float3{ 0,0,7 }, material, this));
}

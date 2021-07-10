#include "scene.h"

Scene::Scene(int& resolutionx, int& resolutiony) :resolutionx(resolutionx), resolutiony(resolutiony)
{
	skyimg.Load(skyimg_path);

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
	objs.add(new light(rt::float3{ -5,  5,  -5 }, 7.0f, this, 1));
	objs.add(new plane(rt::float3{ 0.0f,  -5.0f,  0.0f }, m_plane, this));
	objs.add(new sphere(rt::float3{ 0, -4,  15 }, m_sphere, this, 1.0f));
}

rt::Color Scene::tracepixel(int x, int y)
{
	float pointingx = w * (static_cast<float>(x) / resolutionx - 0.5f);
	float pointingy = h * (0.5f - static_cast<float>(y) / resolutiony);

	rt::float3 dir{ pointingx, pointingy, d };
	dir = camera.transformation(dir);
	Ray ray{ camera.position, dir.normalize() };
	float nearest = -1;
	rt::float3 color = GetSkyColor(ray);

	for (int i = 0; i < objs.size(); i++)
	{
		rt::float4 light = objs[i]->Hit(ray, nearest);
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
					dir = camera.transformation({ pointingx, pointingy, d });
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
					dir = camera.transformation({ pointingx, pointingy, d });
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
					dir = camera.transformation({ pointingx, pointingy, d });
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
					dir = camera.transformation({ pointingx, pointingy, d });
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
				color = { light.y,light.z,light.w };
			}
		}
	}
	return translate_color(color);
}

rt::Color Scene::translate_color(rt::float3 c)
{
	return { static_cast<BYTE>(c.z * 255.0f) ,static_cast<BYTE>(c.y * 255.0f) ,static_cast<BYTE>(c.x * 255.0f) };
}

size_t Scene::Select(float x, float y)
{
	float pointingx = w * x;
	float pointingy = h * y;
	rt::float3 dir{ pointingx, pointingy, d };
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

void Scene::AddMesh(const std::string& path)
{
	Mesh* m = new Mesh(path);
	objs.add(new mesh(m, { 0,0,7 }, this));
}

rt::float3 Scene::GetSkyColor(Ray& ray)
{
	if (skyimg.GetHeightMax() != 0)
	{
		float theta, u, v;
		float dist = ray.dir.x * ray.dir.x + ray.dir.z * ray.dir.z;
		v = dist >= 1 ? 0.5 : (ray.dir.y > 0 ? (0.5 + sqrtf(1 - (dist)) / 2.0) : (0.5 - sqrtf(1 - (dist)) / 2.0));
		if (ray.dir.x != 0)
		{
			theta = atan(ray.dir.z / ray.dir.x);
			if (ray.dir.x < 0)
			{
				theta += M_PI;
			}
			else if (ray.dir.z < 0)
			{
				theta += M_PI * 2;
			}
		}
		else
		{
			theta = ray.dir.z > 0 ? M_PI_2 : M_PI_2 * 3;
		}
		
		u = theta / (M_PI * 2);
		int x = skyimg.GetWidthMax() * u; 
		int y = skyimg.GetHeightMax() * v;

		rt::Color color = skyimg.GetData(x, y);

		float a = static_cast<float>(color.b) / 255.0f;
		float b = static_cast<float>(color.g) / 255.0f;
		float c = static_cast<float>(color.r) / 255.0f;
		return{ c,b,a };
	}
	return { 0.4f,0.4f,0.7f };
}

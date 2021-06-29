#include "object.h"
#define CONSTANT 3
/*------------static member declare------------*/
float object::ambient_a = 0.0f;
float object::ambient_b = 0.7f;
float object::ambient_const = (object::ambient_b - object::ambient_a) / 2.0f;
/*------------static member declare------------*/





/*------------object class------------*/
void object::randomize()
{
	float x = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20)) - 10.0f;
	float y = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 5));
	float z = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 30)) + 1.0f + fabs(x);
	float r = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
	float g = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
	float b = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
	float diffuse = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)) / 4;
	float reflect = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
	Material m;
	m.color = { r,g,b };
	m.diffuse = diffuse;
	m.reflectivity = reflect;
	pos = { x,y,z };
	material = m;
}

inline float object::ambient(float s)
{
	return s * ambient_const + ambient_b / 2.0f;
}
/*------------object class------------*/





/*------------sphere class------------*/
//return { distance from the camera, color }
float4 sphere::Hit(Ray ray, float n)
{
	float t = IsHit(ray);
	float3 hit_pos = ray.pos + ray.dir * t;
	//opposite direction
	if (t < 0.00001 || (t > n && n > 0))
	{
		return { -1,0,0,0 };
	}
	if (--ray.hop <= 0)
	{
		return { t , {0,0,0} };
	}
	float3 normal = (hit_pos - pos) / radius;
	float3 Color = { 0,0,0 };
	if (material.transparent)
	{
		Color = scene->sky;
		float d_dot_n = ray.dir.dot(normal);
		float sin_1 = sqrtf(1 - d_dot_n * d_dot_n);
		float n1 = 1;
		float sin_2 = n1 * sin_1 / material.refractive;
		if (sin_2 > 1)//total internal reflection
		{
			return { 0.1f,0,0,0 };
		}
		float x = -d_dot_n * sin_2 * rsqrt(1 - sin_2 * sin_2);
		float3 dir_refract = normal.cross(ray.dir.cross(normal)).normalize() * x - normal * sqrtf(1 - x * x);


		float b = 2.0f * (dir_refract.dot(hit_pos - pos));
		float c = hit_pos.dot(hit_pos) + pos.dot(pos) - 2.0f * hit_pos.dot(pos) - radius2;
		float d = b * b - 4.0f * c;

		if (d < 0.00001)//this shoudn't happen at all
		{
			return { 0.1f,0,0,0 };
		}
		float t = (-b + sqrtf(d)) / 2.0f;
		float3 new_hit_pos = hit_pos + dir_refract * t;

		float3 new_normal = (pos - new_hit_pos) / radius;


		d_dot_n = dir_refract.dot(new_normal);
		sin_1 = sqrtf(1 - d_dot_n * d_dot_n);
		n1 = 1;
		sin_2 = material.refractive * sin_1 / n1;
		if (sin_2 > 1)//total internal reflection
		{
			sin_2 = 1;
		}
		x = -d_dot_n * sin_2 * rsqrt(1 - sin_2 * sin_2);

		ray.dir = new_normal.cross(dir_refract.cross(new_normal)).normalize() * x - new_normal * sqrtf(1 - x * x);
		ray.pos = new_hit_pos;
	}
	else
	{

		if (material.diffuse != 0)
		{
			float diffuse_r = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)) * material.diffuse;
			float diffuse_phi = static_cast<float>(rand());
			float diffuse_theta = static_cast<float>(rand());

			normal.x += diffuse_r * cos(diffuse_phi) * sin(diffuse_theta);
			normal.y += diffuse_r * sin(diffuse_phi) * sin(diffuse_theta);
			normal.z += diffuse_r * cos(diffuse_theta);
			normal = normal.normalize();
		}

		ray.dir = (normal * (normal.dot(ray.pos - hit_pos)) * 2.0f + hit_pos - ray.pos).normalize();
		ray.pos = hit_pos;
	}
	
	float3 ambient_light = material.color* ambient(normal.dot(scene->light_dir));

	for (int i = 0; i < scene->objs.size(); i++)
	{
		if (scene->objs[i] == this) { continue; }
		Ray ray{ hit_pos,scene->light_dir };
		if ((scene->objs[i]->Type != _Type::_light) && (scene->objs[i]->IsHit(ray) > 0.00001))
		{

			if (scene->objs[i]->material.transparent)
			{
				ambient_light = ambient_light * scene->objs[i]->material.color * 0.5;
				continue;
			}
			else
			{
				ambient_light = ambient_light * 0.5;
			}
			break;
		}
	}
	
	Color = scene->sky * (1 - material.ambient);

	float nearest = -1;
	
	for (int i = 0; i < scene->objs.size(); i++)
	{
		if (scene->objs[i] == this) { continue; }
		float4 light = scene->objs[i]->Hit(ray, nearest);
		if (light.x > 0.00001)
		{
			if (light.x < nearest || nearest < 0)
			{
				nearest = light.x;
				Color = float3{ light.y,light.z,light.w };
			}
		}
	}
	float3 reflection = Color * material.color;

	reflection = reflection * material.reflectivity;
	return { t, reflection + ambient_light * material.ambient };
}

void sphere::randomize()
{
	float rand_radius = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 3.0f));
	radius = rand_radius;
	radius2 = radius * radius;
	object::randomize();
	pos.y = radius - 5;
}
/*------------sphere class------------*/





/*------------light class------------*/
float4 light::Hit(Ray ray, float n)
{

	float t = IsHit(ray);
	if (t < 0.00001 || (t > n && n > 0))
	{
		return { -1,0,0,0 };
	}
	if (--ray.hop <= 0)
	{
		return { t , {0,0,0} };
	}
	float m = ray.dir.dot(pos - ray.pos) - t;
	float x =  m / radius + 0.6f;

	return { t, material.color*x };
}

void light::randomize()
{
	float rand_radius = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 3.0f));
	radius = rand_radius;
	radius2 = radius * radius;
	object::randomize();
}
/*------------light class------------*/





/*------------plane class------------*/
float4 plane::Hit(Ray ray, float n)
{

	float t = IsHit(ray);
	if (t < 0.000027 || (ray.dir.y == 0) || (t > n && n > 0))
	{
		return { -1, float3{0,0,0} };
	}
	if (--ray.hop <= 0)
	{
		return { t, {0,0,0} };
	}
	float3 hit_pos = ray.pos + ray.dir * t;
	float3 ambient_light;
	if (material.diffuse != 0)
	{
		float diffuse_r = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)) * material.diffuse;
		float diffuse_phi = static_cast<float>(rand());
		float diffuse_theta = static_cast<float>(rand());

		float3 normal = float3{ diffuse_r * cos(diffuse_phi) * sin(diffuse_theta) , diffuse_r * sin(diffuse_phi) * sin(diffuse_theta) + 1.0f, diffuse_r * cos(diffuse_theta) }.normalize();
		ray.dir = (normal * (normal.dot(ray.pos - hit_pos)) * 2.0f + hit_pos - ray.pos).normalize();
		ambient_light = material.color * ambient(normal.dot(scene->light_dir));
	}
	else
	{
		ray.dir.y = -ray.dir.y;
		ambient_light = material.color * ambient(scene->light_dir.y);
	}
	ray.pos = hit_pos;

	for (int i = 0; i < scene->objs.size(); i++)
	{
		if (scene->objs[i] == this) { continue; }
		Ray ray{ hit_pos,scene->light_dir };
		if ((!scene->objs[i]->material.transparent) && (scene->objs[i]->IsHit(ray) > 0.00001))
		{

			if (scene->objs[i]->material.transparent)
			{
				ambient_light = ambient_light * scene->objs[i]->material.color*0.5;
				continue;
			}
			else
			{
				ambient_light = ambient_light * 0.5;
			}
			break;
		}
	}

	float nearest = -1;
	float3 Color = { 0,0,0 };
	for (int i = 0; i < scene->objs.size(); i++)
	{
		if (scene->objs[i] == this) { continue; }
		float4 light = scene->objs[i]->Hit(ray, nearest);
		if (light.x > 0.00001)
		{
			if (light.x < nearest || nearest < 0)
			{
				nearest = light.x;
				Color = float3{ light.y,light.z,light.w };
			}
		}
	}

	float3 reflection = Color* material.color;
	if (nearest > 0)
	{
		float factor = 1 / nearest * CONSTANT;
		if (factor < 1)
		{
			reflection = reflection * factor;
		}
	}
	return { t, reflection * material.reflectivity + ambient_light };
}

void plane::randomize()
{
	return;
}
/*------------plane class------------*/





/*------------mesh class------------*/
mesh::mesh(Mesh* M, float3 Position, Material Mat, Scene* scene) :m(M), object(Position, Mat, scene) {
	Type = _Type::_mesh; vertices = m->vertices;
};

float mesh::IsHit(Ray ray)
{

	float b = 2.0f * (ray.dir.dot(ray.pos - pos));
	float c = ray.pos.dot(ray.pos) + pos.dot(pos) - 2.0f * ray.pos.dot(pos) - m->radius2;
	float d = b * b - 4.0f * c;
	//the sphere is not Hit
	if (d < 0)
	{
		return -1;
	}

	float nearest = -1;
	for (int i = 0; i < m->indices.size(); i += 3)
	{
		float t = IsHitTriangle(ray, vertices[m->indices[i]] + pos, vertices[m->indices[i + 1]] + pos, vertices[m->indices[i + 2]] + pos, nearest);
		if (t > 0.00001)
		{
			if (t < nearest || nearest < 0)
			{
				nearest = t;
			}
		}
	}
	return nearest;
}

inline mesh::Mesh_Hit_Type mesh::_IsHit(Ray ray)
{
	float b = 2.0f * (ray.dir.dot(ray.pos - pos));
	float c = ray.pos.dot(ray.pos) + pos.dot(pos) - 2.0f * ray.pos.dot(pos) - m->radius2;
	float d = b * b - 4.0f * c;
	//the sphere is not Hit
	if (d < 0.00001)
	{
		return { -1, 0 };
	}

	float nearest = -1;
	int hit_index = 0;
	for (int i = 0; i < m->indices.size(); i += 3)
	{
		float t = IsHitTriangle(ray, vertices[m->indices[i]] + pos, vertices[m->indices[i + 1]] + pos, vertices[m->indices[i + 2]] + pos, nearest);
		if (t > 0.00001)
		{
			if (t < nearest || nearest < 0)
			{
				nearest = t;
				hit_index = i;
			}
		}

	}
	return Mesh_Hit_Type{ nearest, hit_index };
}

float4 mesh::Hit(Ray ray, float n)
{
	Mesh_Hit_Type Hit = _IsHit(ray);
	float t = Hit.t;
	int hit_index = Hit.hit_index;
	if (t < 0.00001 || (t > n && n > 0))
	{
		return { -1,0,0,0 };
	}

	if (--ray.hop <= 0)
	{
		return { t , {0,0,0} };
	}
	float3 a = vertices[m->indices[hit_index]];
	float3 b = vertices[m->indices[hit_index+1]];
	float3 c = vertices[m->indices[hit_index+2]];
	float3 A = b - a;
	float3 B = c - a;
	float3 normal = A.cross(B).normalize();
	float3 hit_pos = ray.pos + ray.dir * t;
	float3 ambient_light = material.color * ambient(-normal.dot(scene->light_dir));

	for (int i = 0; i < scene->objs.size(); i++)
	{
		if (scene->objs[i] == this)
		{
			continue;
		}
		Ray ray{ hit_pos,scene->light_dir };
		if ((scene->objs[i]->Type!= _Type::_light) && (scene->objs[i]->IsHit(ray) > 0.00001))
		{
			
			if (scene->objs[i]->material.transparent)
			{
				ambient_light = ambient_light * scene->objs[i]->material.color;
			}
			else
			{
				ambient_light = ambient_light * 0.5;
			}
			break;
		}
	}

	

	if (material.transparent)
	{
		float3 light = Refract(ray, hit_pos, normal, hit_index);
		return { t, light };
	}
	else
	{
		if (material.diffuse != 0)
		{
			float diffuse_r = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)) * material.diffuse;
			float diffuse_phi = static_cast<float>(rand());
			float diffuse_theta = static_cast<float>(rand());

			normal.x += diffuse_r * cos(diffuse_phi) * sin(diffuse_theta);
			normal.y += diffuse_r * sin(diffuse_phi) * sin(diffuse_theta);
			normal.z += diffuse_r * cos(diffuse_theta);
			normal = normal.normalize();
		}

		ray.dir = (normal * (normal.dot(ray.pos - hit_pos)) * 2.0f + hit_pos - ray.pos).normalize();
		ray.pos = hit_pos;
	}

	
	float nearest = -1;
	float3 Color = { 0,0,0 };
	for (int i = 0; i < scene->objs.size(); i++)
	{
		if (scene->objs[i] == this)
		{
			continue;
		}
		float4 light = scene->objs[i]->Hit(ray, nearest);
		if (light.x > 0.00001)
		{
			if (light.x < nearest || nearest < 0)
			{
				nearest = light.x;
				Color = float3{ light.y,light.z,light.w };
			}
		}
	}

	float3 reflection = Color * material.color;
	if (nearest > 0)
	{
		float factor = 1 / nearest * CONSTANT;
		if (factor < 1)
		{
			reflection = reflection * factor;
		}
	}
	return { t, reflection * material.reflectivity + ambient_light };
}

float3 mesh::Refract(Ray ray, float3 hit_pos, float3 normal, int hit_index)
{
	if (--ray.hop <= 0)
	{
		return { 1,1,1 };
	}

	float d_dot_n = ray.dir.dot(normal);
	float sin_1 = sqrtf(1 - d_dot_n * d_dot_n);
	float n1 = 0;
	float n2 = 0;

	if (d_dot_n > 0.00001)//inner -> outer
	{
		n1 = material.refractive;
		n2 = 1;
	}
	else//outer -> inner
	{
		n1 = 1;
		n2 = material.refractive;
	}
	float sin_2 = n1 * sin_1 / n2;
	
	float3 color = {1,0,1};
	if (sin_2 > 1)//total internal reflection
	{

		ray.dir = (normal * (normal.dot(ray.pos - hit_pos)) * 2.0f + hit_pos - ray.pos).normalize();
		ray.pos = hit_pos;
		float4 light = Hit(ray, -1);
		if (light.x > 0.00001)
		{
			color.x = light.y;
			color.y = light.z;
			color.z = light.w;
		}
	}
	else
	{
		if (material.refractive !=1 )
		{
			float x = -d_dot_n * sin_2 * rsqrt(1 - sin_2 * sin_2);
			ray.dir = normal.cross(ray.dir.cross(normal)).normalize() * x - normal * sqrtf(1 - x * x);
			if (d_dot_n > 0.00001) { ray.dir = -ray.dir; }
		}
		ray.pos = hit_pos;

		float nearest = -1;

		if (d_dot_n > 0.00001)//inner -> outer
		{
			inner_to_outer:
			color = scene->sky;
			for (int i = 0; i < scene->objs.size(); i++)
			{
				float4 light = scene->objs[i]->Hit(ray, nearest);
				if (light.x > 0.00001)
				{
					if (light.x < nearest || nearest < 0)
					{
						nearest = light.x;
						color = float3{ light.y,light.z,light.w };
					}
				}
			}
		}
		else//outer -> inner
		{
			for (int i = hit_index; i < hit_index + 3; i++)
			{
				float3 dif = ray.pos - vertices[m->indices[i]] - pos;
				if (dif.dot(dif) < 0.008)
				{
					goto inner_to_outer;
				}
			}

			float4 light = Hit(ray, nearest);
			if (light.x > 0.00001)
			{
				if (light.x < nearest || nearest < 0)
				{
					nearest = light.x;
					color = float3{ light.y,light.z,light.w };
				}
			}
		}
	}
	return color;
}

void mesh::Rotatation(float r, float p, float y)
{
	roll = r;
	pitch = p;
	yaw = y;

	float _roll = r * float(M_PI) / 180.0f;
	float _pitch = p * float(M_PI) / 180.0f;
	float _yaw = y * float(M_PI) / 180.0f;

	float a = cos(_yaw);
	float b = cos(_pitch);
	float c = sin(_pitch);
	float d = sin(_roll);
	float e = sin(_yaw);
	float f = cos(_roll);
	float3 m_1 = { a * b, a * c * d - e * f, a * c * f + e * d };
	float3 m_2 = { e * b, e * c * d + a * f, e * c * f - a * d };
	float3 m_3 = { -c, b * d, b * f };
	for (int i = 0; i < m->vertices.size(); i++)
	{
		float3 p = m->vertices[i];
		float x = p.dot(m_1);
		float y = p.dot(m_2);
		float z = p.dot(m_3);
		vertices[i] = { x,y,z };
	}
}

void mesh::randomize()
{
	object::randomize();

	Rotatation(static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 360)),
		static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 360)),
		static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 360)));
	pos.y = -5 + m->radius2;
}
/*------------mesh class------------*/
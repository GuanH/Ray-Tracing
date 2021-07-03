#pragma once
#define _USE_MATH_DEFINES
#include"meshloader.h"
#include"object_wrapper.h"
#include"ray.h"
#include"camera.h"
#include<cmath>
#include<vector>
#include<Windows.h>
#include<sstream>

class Mesh;
class Scene
{
public:
	Scene(int& resolutionx, int& resolutiony);
	rt::Color tracepixel(int x, int y);
	rt::Color translate_color(rt::float3 c);
	size_t Select(float x, float y);
	void AddMesh(const std::string& path);
	rt::float3 GetSkyColor(Ray &ray);
	rt::float3 light_dir = rt::float3{ -1, 0.5, 0 }.normalize();
	object_wrapper objs;
	Camera camera = Camera{ rt::float3{0,0,0},0,0,0 };
	float w = 2.0f;
	float h = 2.0f;
	float d = 2.0f;
	size_t selected = -1;
private:
	int& resolutionx;
	int& resolutiony;
	const std::string skyimg_path = "SkyDome.bmp";
	rt::BMP skyimg;
};
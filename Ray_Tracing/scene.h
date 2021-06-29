#pragma once
#include"meshloader.h"
#include"object_wrapper.h"
#include"ray.h"
#include"camera.h"
#include<vector>
#include<Windows.h>
#include<sstream>

class Mesh;
class Scene
{
public:
	Scene(int& resolutionx, int& resolutiony);
	Color tracepixel(int x, int y);
	Color translate_color(float3 c);
	size_t Select(float x, float y);
	void AddMesh(std::string path);
	float3 sky = { 0.1411f, 0.5529f, 0.9686f };//default color : sky blue
	float3 light_dir = float3{ -1,1,-1 }.normalize();
	object_wrapper objs;
	Camera camera = Camera{ float3{0,0,0},0,0,0 };
	float w = 2.0f;
	float h = 2.0f;
	float d = 1.0f;
	size_t selected = -1;
private:
	int& resolutionx;
	int& resolutiony;
};
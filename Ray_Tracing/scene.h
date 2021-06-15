#pragma once
#include"ray.h"
#include"object.h"
#include<vector>
#include<Windows.h>
class object;
struct vec3;
struct Color
{
	BYTE b;
	BYTE g;
	BYTE r;
};
class Scene
{
public:
	Scene(int& resolutionx, int& resolutiony);
	Color&& tracepixel(int x, int y);
	Color&& translate_color(vec3 c);
	vec3 sky = { 0.1411f, 0.5529f, 0.9686f };//default color : sky blue
	std::vector<object*> objs;
	float w = 1.0f;
	float h = 1.0f;
	float d = 1.0f;
private:
	int& resolutionx;
	int& resolutiony;
};
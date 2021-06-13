#pragma once
#include"object.h"
#include<vector>
#include<Windows.h>
class object;
struct vec3;
struct color
{
	BYTE b;
	BYTE g;
	BYTE r;
};
class Scene
{
public:
	Scene(int& resolutionx, int& resolutiony);
	color&& tracepixel(int x, int y);
	color&& translate_color(vec3 c);
	std::vector<object*> objs;
private:
	int& resolutionx;
	int& resolutiony;
};
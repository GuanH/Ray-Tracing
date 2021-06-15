#pragma once
#include<vector>
#include<Windows.h>
#include<CommCtrl.h>
#include<ctime>
#include<sstream>
#include"object.h"
class Graphic
{
public:
	Graphic(HWND hwnd, int x, int y);
	void render();
	void randomize_object();
	void upadatewndsize(int x, int y);
	void updateresolution(int x, int y);
	Scene* sc;
private:
	HWND hwnd;
	HWND hbutton_render = 0;
	HWND hbutton_random = 0;
	HWND htext_res_x = 0;
	HWND htext_res_y = 0;
	HDC hdc;
	BITMAPINFO BitmapInfo = {0};
	int resolutionx = 256;
	int resolutiony = 256;
	int wndwidth;
	int wndheight;
	std::vector<BYTE> result;
	friend class wnd;
};
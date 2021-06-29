#pragma once
#define _USE_MATH_DEFINES
#define TOP_MARGIN 60
#define BUTTON_ID_RENDER 0
#define BUTTON_ID_RANDOM 1
#define BUTTON_ID_THREADSET 2
#define BUTTON_ID_TRANSPARENTCHECK 3
#define BUTTON_ID_SET 4
#define BUTTON_ID_DELETE 5
#define BUTTON_ID_LOAD 6
#define BUTTON_ID_COPY 7
#define BUTTON_ID_RANDOMIZECHECK 8
#include"scene.h"
#include<vector>
#include<Windows.h>
#include<CommCtrl.h>
#include<ctime>
#include<sstream>
#include<cmath>
#include<thread>

class Graphic
{
public:
	Graphic(HWND hwnd, int x, int y);
	void render();
	void randomize_object();
	void upadatewndsize(int x, int y);
	void updateresolution(int x, int y);
	void RenderButtonDown();
	void RandomButtonDown();
	void SetThreadButtonDown();
	void SelectTransparentCheck();
	void SelectSet();
	void SelectDelete();
	void LoadMeshButtonDown();
	void CopyButtonDown();
	void SelectRandomizeCheck();
	void Select(float x, float y);
	bool isRender = true;
	Scene* sc;
private:
	HWND hwnd;
	HWND hbutton_render = 0;
	HWND hbutton_random = 0;
	HWND htext_res_x = 0;
	HWND htext_res_y = 0;
	HWND htext_cam_x = 0;
	HWND htext_cam_y = 0;
	HWND htext_cam_z = 0;
	HWND htext_cam_roll = 0;
	HWND htext_cam_pitch = 0;
	HWND htext_cam_yaw = 0;
	HWND htext_thread = 0;
	HWND hbutton_thread = 0;

	HWND htext_select_pos_x = 0;
	HWND htext_select_pos_y = 0;
	HWND htext_select_pos_z = 0;
	HWND htext_select_diffuse = 0;
	HWND htext_select_reflectivity = 0;
	HWND htext_select_ambient = 0;
	HWND htext_select_refractive = 0;
	HWND htext_select_color_r = 0;
	HWND htext_select_color_g = 0;
	HWND htext_select_color_b = 0;
	HWND hbutton_select_transparent = 0;
	HWND hbutton_select_set = 0;
	HWND hbutton_select_delete = 0;
	HWND hbutton_copy = 0;
	HWND hbutton_load_mesh = 0;

	HWND htext_select_roll = 0;
	HWND htext_select_pitch = 0;
	HWND htext_select_yaw = 0;

	HWND hbutton_select_randomlock = 0;

	HDC hdc;
	BITMAPINFO BitmapInfo = {0};
	int resolutionx = 512;
	int resolutiony = 512;
	int wndwidth;
	int wndheight;
	std::vector<BYTE> result;
	std::vector<std::thread> threads;
	void ProcessPixels();
	void UpdateUI();
	friend class wnd;
};
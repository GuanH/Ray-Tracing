#pragma once
#include<vector>
#include<windowsx.h>
#include<string>
#include"graphic.h"
class wnd
{
public:
	wnd(LPCTSTR name, LPCTSTR title, HINSTANCE& hInstance, int& nCmdshow, unsigned width, unsigned height);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	const HWND& gethandle();
	void processmessage();
private:
	HWND hwnd = 0;
	HINSTANCE hInstance;
	int wndwidth;
	int wndheight;
	void setwndsize();
	Graphic* gfx;
};
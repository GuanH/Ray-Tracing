#pragma once
#include"wnd.h"
#include<math.h>
int __stdcall WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	wnd Wnd("RayTracing", "Ray Tracing", hInstance, nShowCmd, 1200, 900);
	Wnd.processmessage();
}
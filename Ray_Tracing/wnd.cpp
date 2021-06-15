#pragma comment(lib,"Msimg32.lib")
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#include "wnd.h"

wnd::wnd(LPCTSTR name, LPCTSTR title, HINSTANCE& hInstance, int& nCmdshow, unsigned width, unsigned height)
{
	this->wndwidth = 0;
	this->wndheight = 0;
	WNDCLASSEX wndclass = { 0 };
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbWndExtra = sizeof(wnd*);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = this->WndProc;
	wndclass.lpszClassName = name;
	wndclass.hbrBackground = nullptr;
	RegisterClassEx(&wndclass);
	this->hwnd = CreateWindowEx(0, name, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, this);
	ShowWindow(this->hwnd, nCmdshow);
	UpdateWindow(this->hwnd);
	this->hInstance = hInstance;
	gfx = new Graphic{ hwnd, wndwidth, wndheight };
}
LRESULT wnd::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	wnd* pthis;
	if (msg == WM_CREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pthis = static_cast<wnd*>(lpcs->lpCreateParams);
		pthis->hwnd = hwnd;
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pthis));
		pthis->setwndsize();
	}
	else
	{
		pthis = reinterpret_cast<wnd*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	}

	if (pthis)
	{
		switch (msg)
		{
			case WM_DESTROY: {PostQuitMessage(0); break; }
			case WM_PAINT: {pthis->setwndsize(); break; }
			case WM_COMMAND:
			{
				switch (HIWORD(wParam))
				{
					case BN_CLICKED:
					{
						switch (LOWORD(wParam))
						{
							case 0:
							{
								char s[10];
								GetWindowText(pthis->gfx->htext_res_x, s, 10);
								if(strlen(s)>0)
								{
									int x = std::stoi(s);
									GetWindowText(pthis->gfx->htext_res_y, s, 10);
									if (strlen(s) > 0)
									{
										int y = std::stoi(s);
										if (x > 0 && y > 0)
										{
											pthis->gfx->updateresolution(x, y);
										}
									}
								}
								pthis->render = true;
								break;
							}
							case 1:
							{
								pthis->gfx->randomize_object();
								pthis->render = true;
								break;
							}
						}
						break;
					}
				}
				break;
			}
			default: {return DefWindowProc(hwnd, msg, wParam, lParam); break; }
		}
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
const HWND& wnd::gethandle()
{
	return this->hwnd;
}
void wnd::processmessage()
{
	HDC hdc = GetDC(hwnd);
	BITMAPINFO bi = { 0 };
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = 256;
	bi.bmiHeader.biHeight = 256;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = 0;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (render)
		{
			gfx->render();
			render = false;
		}
	}
}
void wnd::setwndsize()
{
	RECT r;
	if (GetWindowRect(this->hwnd, &r))
	{
		this->wndwidth = r.right - r.left;
		this->wndheight = r.bottom - r.top;
	}
	if (gfx)
	{
		gfx->upadatewndsize(wndwidth, wndheight);
	}
}
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
	{
		WNDCLASSEX wndclass = { 0 };
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.cbWndExtra = sizeof(wnd*);
		wndclass.hInstance = hInstance;
		wndclass.lpfnWndProc = this->WndProc;
		wndclass.lpszClassName = name;
		wndclass.hbrBackground = nullptr;
		wndclass.style = CS_DBLCLKS;
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		RegisterClassEx(&wndclass);
	}	
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
			case WM_SIZE:
			{
				pthis->setwndsize();
				break; 
			}
			case WM_COMMAND:
			{
				switch (HIWORD(wParam))
				{
					case BN_CLICKED:
					{
						switch (LOWORD(wParam))
						{
							case BUTTON_ID_RENDER:
							{
								pthis->gfx->RenderButtonDown();
								break;
							}
							case BUTTON_ID_RANDOM:
							{
								pthis->gfx->RandomButtonDown();
								break;
							}
							case BUTTON_ID_THREADSET:
							{
								pthis->gfx->SetThreadButtonDown();
								break;
							}
							case BUTTON_ID_TRANSPARENTCHECK:
							{
								pthis->gfx->SelectTransparentCheck();
								break;;
							}
							case BUTTON_ID_SET:
							{
								pthis->gfx->SelectSet();
								break;
							}
							case BUTTON_ID_DELETE:
							{
								pthis->gfx->SelectDelete();
								break;
							}
							case BUTTON_ID_LOAD:
							{
								pthis->gfx->LoadMeshButtonDown();
								break;
							}
							case BUTTON_ID_COPY:
							{
								pthis->gfx->CopyButtonDown();
								break;
							}
							case BUTTON_ID_RANDOMIZECHECK:
							{
								pthis->gfx->SelectRandomizeCheck();
							}
						}
						break;
					}
				}
				break;
			}
			case WM_LBUTTONDBLCLK :
			{
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam) - TOP_MARGIN;

				float a = static_cast<float>(x) / pthis->wndwidth - 0.5f;
				float b = 0.5f - static_cast<float>(y) / (pthis->wndheight-60);
				pthis->gfx->Select(a, b);
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
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (gfx->isRender)
		{
			gfx->render();
			gfx->isRender = false;
		}
		Sleep(1);
	}
}
void wnd::setwndsize()
{
	RECT r;
	if (GetClientRect(this->hwnd, &r))
	{
		this->wndwidth = r.right - r.left;
		this->wndheight = r.bottom - r.top;
	}
	if (gfx)
	{
		gfx->upadatewndsize(wndwidth, wndheight);
	}
}
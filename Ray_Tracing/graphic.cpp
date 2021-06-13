#include "graphic.h"

Graphic::Graphic(HWND hwnd, int x, int y) :hwnd(hwnd), wndwidth(x - 30), wndheight(y - 30)
{
	int pad = (4 - ((resolutionx * 3) % 4)) % 4;
	int width = resolutionx * 3 + pad;
	result.resize(width * resolutiony);
	hdc = GetDC(hwnd);
	htext_res_x = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER | ES_NUMBER, 0, 7, 80, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_res_y = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER | ES_NUMBER, 80, 7, 80, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hbutton_render = CreateWindow("BUTTON", "Render", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 160, 0, 80, 30, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	Edit_SetCueBannerText(htext_res_x, L"Res_X");
	Edit_SetCueBannerText(htext_res_y, L"Res_Y");
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biWidth = resolutionx;
	BitmapInfo.bmiHeader.biHeight = resolutiony;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 24;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	BitmapInfo.bmiHeader.biSizeImage = 0;
	BitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	BitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	BitmapInfo.bmiHeader.biClrUsed = 0;
	BitmapInfo.bmiHeader.biClrImportant = 0;


	memset(result.data(), 0, width * resolutiony);
	sc = new Scene{resolutionx, resolutiony};
}

void Graphic::render()
{
	/*Do render stuff*/
	int pad = (4 - ((resolutionx * 3) % 4)) % 4;
	int width = resolutionx * 3 + pad;
	clock_t timer = clock();
	for (int i = 0; i < resolutiony; i++)
	{
		for (int j = 0; j < resolutionx; j++)
		{
			color r = sc->tracepixel(j, i);
			int index = (width * i + j * 3);
			result[index] = r.b;
			result[index + 1] = r.g;
			result[index + 2] = r.r;
		}
	}
	std::stringstream ss;
	ss << (double)(clock() - timer) / CLOCKS_PER_SEC << '\n';
	OutputDebugString(ss.str().c_str());
	/*Do render stuff*/

	BitBlt(hdc, 0, 0, wndwidth, wndheight, NULL, 0, 0, WHITENESS);//clear the window
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, resolutionx, resolutiony);
	SetDIBits(hdc, bitmap, 0, resolutiony, result.data(), &BitmapInfo, DIB_RGB_COLORS);
	HDC memDC = CreateCompatibleDC(hdc);
	SelectObject(memDC, bitmap);
	int size = wndwidth > wndheight ? wndheight : wndwidth;
	StretchBlt(hdc, 0, 30, size, size, memDC, 0, 0, resolutionx, resolutiony, SRCCOPY);
	DeleteObject(memDC);
	DeleteObject(bitmap);

	RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);//redraw the textbox and the button
}

void Graphic::upadatewndsize(int x, int y)
{
	wndwidth = x - 30;
	wndheight = y - 30;
}

void Graphic::updateresolution(int x, int y)
{
	resolutionx = x;
	resolutiony = y;
	BitmapInfo.bmiHeader.biWidth = resolutionx;
	BitmapInfo.bmiHeader.biHeight = resolutiony;
	int pad = (4 - ((resolutionx * 3) % 4)) % 4;
	int width = resolutionx * 3 + pad;
	result.resize(width * resolutiony);
	memset(result.data(), 0, width * resolutiony);
}
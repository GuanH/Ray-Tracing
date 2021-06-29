#include "graphic.h"
Graphic::Graphic(HWND hwnd, int x, int y) :hwnd(hwnd), wndwidth(x), wndheight(y)
{
	int pad = (4 - ((resolutionx * 3) % 4)) % 4;
	int width = resolutionx * 3 + pad;
	result.resize(width * resolutiony);
	hdc = GetDC(hwnd);

	htext_res_x =		CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER | ES_NUMBER, 0,  7, 80, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_res_y =		CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER | ES_NUMBER, 80, 7, 80, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hbutton_render =	CreateWindow("BUTTON", "Render", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 160, 0, 80, 30, hwnd, (HMENU)BUTTON_ID_RENDER, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hbutton_render =	CreateWindow("BUTTON", "Random", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 240, 0, 80, 30, hwnd, (HMENU)BUTTON_ID_RANDOM, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	Edit_SetCueBannerText(htext_res_x, L"Res_X");
	Edit_SetCueBannerText(htext_res_y, L"Res_Y");

	htext_cam_x =		CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER , 0,   38, 40, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_cam_y =		CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER , 40,  38, 40, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_cam_z =		CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER , 80,  38, 40, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_cam_roll =	CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER , 120, 38, 40, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_cam_pitch =	CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER , 160, 38, 40, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_cam_yaw =		CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER , 200, 38, 40, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	Edit_SetCueBannerText(htext_cam_x, L"x");
	Edit_SetCueBannerText(htext_cam_y, L"y");
	Edit_SetCueBannerText(htext_cam_z, L"z");
	Edit_SetCueBannerText(htext_cam_roll, L"roll");
	Edit_SetCueBannerText(htext_cam_pitch, L"pitch");
	Edit_SetCueBannerText(htext_cam_yaw, L"yaw");

	htext_select_pos_x =			CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 260, 38, 30, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_pos_y =			CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 290, 38, 30, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_pos_z =			CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 320, 38, 30, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_diffuse =			CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 350, 38, 50, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_reflectivity =		CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 400, 38, 50, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_ambient =			CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 450, 38, 50, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_refractive =		CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 500, 38, 50, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_color_r =			CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 550, 38, 30, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_color_g =			CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 580, 38, 30, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_color_b =			CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 610, 38, 30, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	
	htext_select_roll =				CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 640, 38, 40, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_pitch =			CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 680, 38, 40, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	htext_select_yaw =				CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER, 720, 38, 40, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

	hbutton_select_transparent =	CreateWindow("BUTTON", "transparent",	WS_VISIBLE | WS_CHILD | BS_CHECKBOX,	760, 38, 95, 22,	hwnd, (HMENU)BUTTON_ID_TRANSPARENTCHECK, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hbutton_select_set =			CreateWindow("BUTTON", "Set",			WS_VISIBLE | WS_CHILD | WS_BORDER,		870, 38, 30, 22,	hwnd, (HMENU)BUTTON_ID_SET, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hbutton_select_delete =			CreateWindow("BUTTON", "Del",			WS_VISIBLE | WS_CHILD | WS_BORDER,		900, 38, 30, 22,	hwnd, (HMENU)BUTTON_ID_DELETE, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hbutton_copy =					CreateWindow("BUTTON", "Copy",			WS_VISIBLE | WS_CHILD | WS_BORDER,		930, 38, 50, 22,	hwnd, (HMENU)BUTTON_ID_COPY, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hbutton_load_mesh =				CreateWindow("BUTTON", "Load Mesh",		WS_VISIBLE | WS_CHILD | WS_BORDER,		820, 7,  80, 22,	hwnd, (HMENU)BUTTON_ID_LOAD, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hbutton_select_randomlock =		CreateWindow("BUTTON", "Randomized",	WS_VISIBLE | WS_CHILD | BS_CHECKBOX,	710, 7, 100, 22,	hwnd, (HMENU)BUTTON_ID_RANDOMIZECHECK, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

	
	Edit_SetCueBannerText(htext_select_pos_x, L"x");
	Edit_SetCueBannerText(htext_select_pos_y, L"y");
	Edit_SetCueBannerText(htext_select_pos_z, L"z");
	Edit_SetCueBannerText(htext_select_diffuse, L"diffuse");
	Edit_SetCueBannerText(htext_select_reflectivity, L"reflect");
	Edit_SetCueBannerText(htext_select_ambient, L"ambient");
	Edit_SetCueBannerText(htext_select_refractive, L"refract");
	Edit_SetCueBannerText(htext_select_color_r, L"r");
	Edit_SetCueBannerText(htext_select_color_g, L"g");
	Edit_SetCueBannerText(htext_select_color_b, L"b");

	Edit_SetCueBannerText(htext_select_roll, L"roll");
	Edit_SetCueBannerText(htext_select_pitch, L"pitch");
	Edit_SetCueBannerText(htext_select_yaw, L"yaw");


	htext_thread = CreateWindow("Edit", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_BORDER| ES_NUMBER, 400, 7, 110, 22, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	std::wstringstream ss;
	ss << L"threads (max:" << std::thread::hardware_concurrency() - 1 << ')';
	Edit_SetCueBannerText(htext_thread, ss.str().c_str());
	hbutton_thread = CreateWindow("BUTTON", "Set", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 510, 0, 40, 30, hwnd, (HMENU)BUTTON_ID_THREADSET, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);



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
	sc->w = 2.0f * static_cast<float>(wndwidth) / static_cast<float>(wndheight);

	threads.resize(1);
}

void Graphic::render()
{
	/*Do render stuff*/
#ifdef _DEBUG
	clock_t timer = clock();
#endif
	ProcessPixels();
#ifdef _DEBUG
	std::stringstream ss;
	ss << (float)(clock() - timer) / CLOCKS_PER_SEC << '\n';
	OutputDebugString(ss.str().c_str());
#endif
	/*Do render stuff*/
	upadatewndsize(wndwidth, wndheight);
}

void Graphic::randomize_object()
{
	sc->objs.MakeSphereUpTo(8, sc);
	srand(clock());
	for (int i = 0; i < sc->objs.size(); i++)
	{
		sc->objs.randomize(i);
	}
}

void Graphic::upadatewndsize(int x, int y)
{
	wndwidth = x;
	wndheight = y;
	sc->w = 2.0f * static_cast<float>(wndwidth) / static_cast<float>(wndheight);

	HBITMAP bitmap = CreateCompatibleBitmap(hdc, resolutionx, resolutiony);
	SetDIBits(NULL, bitmap, 0, resolutiony, result.data(), &BitmapInfo, DIB_RGB_COLORS);

	HDC memDC = CreateCompatibleDC(hdc);
	HDC frontDC = CreateCompatibleDC(hdc);
	HBITMAP frontBMP = CreateCompatibleBitmap(hdc, wndwidth, wndheight);
	SelectObject(frontDC, frontBMP);
	SelectObject(memDC, bitmap);
	BitBlt(frontDC, 0, 0, wndwidth, wndheight, NULL, 0, 0, WHITENESS);//clear the window
	
	SetStretchBltMode(frontDC, HALFTONE);
	StretchBlt(frontDC, 0, TOP_MARGIN, wndwidth, wndheight - TOP_MARGIN, memDC, 0, 0, resolutionx, resolutiony, SRCCOPY);
	SetBrushOrgEx(frontDC, 0, 0, NULL);
	BitBlt(hdc, 0, 0, wndwidth, wndheight, frontDC, 0, 0, SRCCOPY);

	DeleteDC(memDC);
	DeleteObject(bitmap);
	DeleteDC(frontDC);
	DeleteObject(frontBMP);
	RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);//redraw the textbox and the button
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

void Graphic::RenderButtonDown()
{
	char s[10];
	int res_x, res_y;
	GetWindowText(htext_res_x, s, 10);
	if (strlen(s) > 0)
	{
		res_x = strtol(s, NULL, 10);
		if (res_x > 0)
		{
			GetWindowText(htext_res_y, s, 10);
			if (strlen(s) > 0)
			{
				res_y = strtol(s, NULL, 10);
				if (res_y > 0)
				{
					updateresolution(res_x, res_y);
				}
			}
		}
	}


	GetWindowText(htext_cam_x, s, 10);
	float x, y, z, roll, pitch, yaw;
	x = (strlen(s) > 0) ? std::stof(s) : 0;

	GetWindowText(htext_cam_y, s, 10);
	y = (strlen(s) > 0) ? std::stof(s) : 0;

	GetWindowText(htext_cam_z, s, 10);
	z = (strlen(s) > 0) ? std::stof(s) : 0;

	GetWindowText(htext_cam_roll, s, 10);
	roll = (strlen(s) > 0) ? std::stof(s) : 0;


	GetWindowText(htext_cam_pitch, s, 10);
	pitch = (strlen(s) > 0) ? std::stof(s) : 0;
	
	GetWindowText(htext_cam_yaw, s, 10);
	yaw = (strlen(s) > 0) ? std::stof(s) : 0;

	roll *= float(M_PI) / 180.0f;
	pitch *= float(M_PI) / 180.0f;
	yaw *= float(M_PI) / 180.0f;
	sc->camera.Update(float3{ x,y,z }, roll, pitch, yaw);

	isRender = true;
}

void Graphic::RandomButtonDown()
{
	randomize_object();
	UpdateUI();
	isRender = true;
}

void Graphic::SetThreadButtonDown()
{
	char buf[10];
	int err = GetWindowText(htext_thread, buf, 3);
	if (err != 0)
	{
		std::stringstream ss;
		ss << buf;
		int s;
		ss >> s;
		int max = std::thread::hardware_concurrency();
		if (s > 0 && s < max)
		{
			threads.resize(s);
		}
	}	
}

void Graphic::SelectTransparentCheck()
{
	UINT state = IsDlgButtonChecked(hwnd, 3);

	if (state == BST_CHECKED)
	{
		CheckDlgButton(hwnd, 3, BST_UNCHECKED);
	}
	else
	{
		CheckDlgButton(hwnd, 3, BST_CHECKED);
	}
	RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void Graphic::SelectSet()
{
	size_t index = sc->selected;

	if (index != -1)
	{
		char s[10];
		float x, y, z, diffuse, reflectivity, ambient, refractive, r, g, b, roll, pitch, yaw;

		GetWindowText(htext_select_pos_x, s, 10);
		x = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_pos_y, s, 10);
		y = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_pos_z, s, 10);
		z = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_diffuse, s, 10);
		diffuse = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_reflectivity, s, 10);
		reflectivity = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_ambient, s, 10);
		ambient = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_refractive, s, 10);
		refractive = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_color_r, s, 10);
		r = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_color_g, s, 10);
		g = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_color_b, s, 10);
		b = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_roll, s, 10);
		roll = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_pitch, s, 10);
		pitch = (strlen(s) > 0) ? std::stof(s) : 0;

		GetWindowText(htext_select_yaw, s, 10);
		yaw = (strlen(s) > 0) ? std::stof(s) : 0;




		object* obj = sc->objs[index];
		obj->pos = { x,y,z };
		obj->material.diffuse = diffuse;
		obj->material.reflectivity = reflectivity;
		obj->material.ambient = ambient;
		obj->material.refractive = refractive;
		obj->material.color = { r,g,b };
		obj->Rotatation(roll, pitch, yaw);
		UINT state = IsDlgButtonChecked(hwnd, 3);

		if (state == BST_CHECKED)
		{
			sc->selected = sc->objs.UpdateTransparent(index, true);
		}
		else
		{
			sc->selected = sc->objs.UpdateTransparent(index, false);
		}

		state = IsDlgButtonChecked(hwnd, 8);

		if (state == BST_CHECKED)
		{
			sc->objs.SetRandomizeLock(index, false);
		}
		else
		{
			sc->objs.SetRandomizeLock(index, true);
		}

		isRender = true;
	}
}

void Graphic::SelectDelete()
{
	size_t index = sc->selected;
	if (index != -1)
	{
		sc->objs.Delete(index);
	}
	isRender = true;
}

void Graphic::LoadMeshButtonDown()
{
	OPENFILENAMEA opf = {0};
	char FilePath[MAX_PATH] = "";
	opf.lStructSize = sizeof(OPENFILENAME);
	opf.hwndOwner = hwnd;
	opf.lpstrFilter = "OBJ Files(*.obj)\0*.obj\0\0";
	opf.nMaxFile = MAX_PATH;
	opf.nMaxFileTitle = MAX_PATH;
	opf.lpstrFileTitle = FilePath;
	opf.lpstrFile = FilePath;
	opf.lpstrTitle = "Select a Mesh";
	opf.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY;

	if (GetOpenFileName(&opf))
	{
		sc->AddMesh(opf.lpstrFile);
	}
	isRender = true;
}

void Graphic::CopyButtonDown()
{
	size_t index = sc->selected;
	if (index != -1)
	{
		sc->objs.Clone(index);
		isRender = true;
	}	
}

void Graphic::SelectRandomizeCheck()
{
	UINT state = IsDlgButtonChecked(hwnd, 8);

	if (state == BST_CHECKED)
	{
		CheckDlgButton(hwnd, 8, BST_UNCHECKED);
	}
	else
	{
		CheckDlgButton(hwnd, 8, BST_CHECKED);
	}
	RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void Graphic::Select(float x,float y)
{
	sc->Select(x, y);
	UpdateUI();	
}

void Graphic::ProcessPixels()
{
	int pad = (4 - ((resolutionx * 3) % 4)) % 4;
	int width = resolutionx * 3 + pad;
	
	size_t chunck_size = resolutiony / threads.size();
	size_t chunck_end = 0;
	
	for (int i = 0; i < threads.size() - 1; i++)
	{
		chunck_end = chunck_size * i + chunck_size;

		threads[i] = std::thread([chunck_size, this, chunck_end, width](int t_i, BYTE* t_result)
		{
			for (int j = int(chunck_size * t_i); j < chunck_end; j++)
			{
				for (int k = 0; k < resolutionx; k++)
				{
					Color r = sc->tracepixel(k, resolutiony - j);
					size_t index = (width * j + k * 3);
					t_result[index] = r.b;
					t_result[index + 1] = r.g;
					t_result[index + 2] = r.r;
				}
			}
		}
		, i, &result[0]);
	}

	threads[threads.size() - 1] = std::thread([this, chunck_end, width](BYTE* t_result)
	{
		for (int i = int(chunck_end); i < resolutiony; i++)
		{
			for (int j = 0; j < resolutionx; j++)
			{
				Color r = sc->tracepixel(j, resolutiony - i);
				int index = (width * i + j * 3);
				t_result[index] = r.b;
				t_result[index + 1] = r.g;
				t_result[index + 2] = r.r;
			}
		}
	},&result[0]);
	for (auto& t : threads)
	{
		t.join();
	}
}

void Graphic::UpdateUI()
{
	size_t index = sc->selected;
	if (index != -1)
	{
		std::string buf;
		std::stringstream ss;

		ss << sc->objs[index]->pos.x;
		ss >> buf;
		ss.clear();
		SetWindowText(htext_select_pos_x, buf.c_str());

		ss << sc->objs[index]->pos.y;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_pos_y, buf.c_str());

		ss << sc->objs[index]->pos.z;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_pos_z, buf.c_str());

		ss << sc->objs[index]->material.diffuse;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_diffuse, buf.c_str());

		ss << sc->objs[index]->material.reflectivity;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_reflectivity, buf.c_str());

		ss << sc->objs[index]->material.ambient;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_ambient, buf.c_str());

		ss << sc->objs[index]->material.refractive;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_refractive, buf.c_str());

		ss << sc->objs[index]->material.color.x;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_color_r, buf.c_str());

		ss << sc->objs[index]->material.color.y;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_color_g, buf.c_str());

		ss << sc->objs[index]->material.color.z;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_color_b, buf.c_str());

		float3 rotation = sc->objs[index]->GetRotation();

		ss << rotation.x;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_roll, buf.c_str());

		ss << rotation.y;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_pitch, buf.c_str());

		ss << rotation.z;
		ss >> buf; ss.clear();
		SetWindowText(htext_select_yaw, buf.c_str());

		if (sc->objs[index]->material.transparent)
		{
			CheckDlgButton(hwnd, 3, BST_CHECKED);
		}
		else
		{
			CheckDlgButton(hwnd, 3, BST_UNCHECKED);
		}

		if (sc->objs.GetRandomizeLock(index))
		{
			CheckDlgButton(hwnd, 8, BST_UNCHECKED);
		}
		else
		{
			CheckDlgButton(hwnd, 8, BST_CHECKED);
		}
	}
	else
	{
		SetWindowText(htext_select_pos_x, NULL);
		SetWindowText(htext_select_pos_y, NULL);
		SetWindowText(htext_select_pos_z, NULL);
		SetWindowText(htext_select_diffuse, NULL);
		SetWindowText(htext_select_reflectivity, NULL);
		SetWindowText(htext_select_ambient, NULL);
		SetWindowText(htext_select_refractive, NULL);
		SetWindowText(htext_select_color_r, NULL);
		SetWindowText(htext_select_color_g, NULL);
		SetWindowText(htext_select_color_b, NULL);
		SetWindowText(htext_select_roll, NULL);
		SetWindowText(htext_select_pitch, NULL);
		SetWindowText(htext_select_yaw, NULL);
		CheckDlgButton(hwnd, 3, BST_UNCHECKED);
		CheckDlgButton(hwnd, 8, BST_UNCHECKED);
	}
	isRender = true;
}

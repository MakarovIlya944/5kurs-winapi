#pragma once
#include "framework.h"
#include "resource.h"

static int count = 0;

class WindowDescription
{
	int id;
	int s_len = 64;
	wchar_t* w_str = new wchar_t[s_len];
	char* c_str = new char[s_len];
	int x = 0;
	int y = 0;

public:
	int one_event, both_event;
	int one_id, both_id;
	bool isChecked = false;
	RECT winRect;
	HWND hwnd;

	WindowDescription() {

	}

	WindowDescription(HWND _hwnd) {
		hwnd = _hwnd;
		id = count++;
	}

	int get_id() { return id; }

	void set_coords(int _x, int _y) {
		x = _x; y = _y;
	}

	void paint(HDC hdc)
	{
		int r = 255, g = 0, b = 0;
		if (isChecked)
		{
			r = 0;
			b = 255;
		}

		SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(r, g, b)));
		GetClientRect(hwnd, &winRect);

		s_len = sprintf_s(c_str, -1, "Coords X[%0.4d] Y[%0.4d]", x, y) + 1;
		MultiByteToWideChar(CP_ACP, 0, c_str, -1, w_str, s_len);
		DrawText(hdc, w_str, -1, &winRect, DT_BOTTOM);

		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, winRect.right, winRect.bottom);
		MoveToEx(hdc, winRect.right, 0, NULL);
		LineTo(hdc, 0, winRect.bottom);
	}
};
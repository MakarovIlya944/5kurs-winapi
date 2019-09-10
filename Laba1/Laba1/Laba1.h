#pragma once
#include "framework.h"
#include "resource.h"

static int count = 0;
// ind or len(all)
static int who_show;

class WindowDescription
{
	int id;
	int s_len = 64;
	wchar_t* w_str = new wchar_t[s_len];
	char* c_str = new char[s_len];
	int x = 0;
	int y = 0;
	int time;
	const int h_max = 43200, m_max = 3600, s_max = 60;

public:
	int one_event, both_event;
	bool isChecked = false;
	RECT winRect;
	HWND hwnd;

	WindowDescription() {

	}

	WindowDescription(HWND _hwnd) {
		hwnd = _hwnd;
		id = count++;
	}

	/*WindowDescription& operator= (const WindowDescription& a)
	{
		WindowDescription ret;
		ret.id = a.id;
		ret.hwnd = a.hwnd;
		return ret;
	}*/

	void init(HWND _hwnd) {
		hwnd = _hwnd;
		id = count++;
	}
	void set_time(SYSTEMTIME t) { time = t.wSecond + (t.wMinute + (t.wHour % 12) * 60) * 60; }
	void set_show(int ind) { who_show = ind; }
	void set_show() { who_show = id; }

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
		SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
		GetClientRect(hwnd, &winRect);
		//SetBkColor(hdc, RGB(255, 255, 255));
		Rectangle(hdc, -1, -1, winRect.right + 1, winRect.bottom + 1);

		if(who_show < 0 || who_show == id)
			s_len = sprintf_s(c_str, -1, "Coords X[%0.4d] Y[%0.4d]", x, y) + 1;
		else
			s_len = sprintf_s(c_str, -1, "Coords dont work here!!") + 1;

		MultiByteToWideChar(CP_ACP, 0, c_str, -1, w_str, s_len);
		DrawText(hdc, w_str, -1, &winRect, DT_BOTTOM);

		/*MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, winRect.right, winRect.bottom);
		MoveToEx(hdc, winRect.right, 0, NULL);
		LineTo(hdc, 0, winRect.bottom);*/

		int R = min(winRect.bottom, winRect.right) / 2;
		int c_x = winRect.right/2, c_y = winRect.bottom/2;
		MoveToEx(hdc, c_x + R, c_y, NULL);
		AngleArc(hdc, c_x, c_y, R, 0, 360);

		float ang = 0, k = R / 3.0, pi = 3.1419 * 2;
		ang = (float)time / h_max * pi;
		MoveToEx(hdc, c_x, c_y, NULL);
		LineTo(hdc, c_x + k * cos(ang), c_y + k * sin(ang));
		ang = (float)(time % m_max) / m_max * pi;
		k = R * 2.0 / 3.0;
		MoveToEx(hdc, c_x, c_y, NULL);
		LineTo(hdc, c_x + k * cos(ang), c_y + k * sin(ang));
		ang = (float)(time % s_max) / s_max * pi;
		k = R;
		MoveToEx(hdc, c_x, c_y, NULL);
		LineTo(hdc, c_x + k * cos(ang), c_y + k * sin(ang));
	}
};
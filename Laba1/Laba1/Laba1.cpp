// Laba1.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Laba1.h"
#include <map>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hWnd_1, hWnd_2;


class WindowDescription
{
	int w_len = 19;
	wchar_t* w_str = new wchar_t[w_len];
	int c_len = 19;
	char* c_str = new char[c_len];
	int x = 0;
	int y = 0;

public:
	bool isChecked = false;
	RECT winRect;
	HWND hwnd;

	WindowDescription() {

	}

	WindowDescription(HWND _hwnd) {
		hwnd = _hwnd;
	}

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

		sprintf_s(c_str, c_len, "Coords X[%d] Y[%d]", x, y);
		MultiByteToWideChar(CP_ACP, 0, c_str, -1, w_str, w_len);
		DrawText(hdc, w_str, -1, &winRect, DT_BOTTOM);

		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, winRect.right, winRect.bottom);
		MoveToEx(hdc, winRect.right, 0, NULL);
		LineTo(hdc, 0, winRect.bottom);
	}
};

std::map<HWND, WindowDescription> windows;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDC_LABA1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA1));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LABA1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
	hWnd_1 = CreateWindowW(szWindowClass, L"Title 1", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	hWnd_2 = CreateWindowW(szWindowClass, L"Title 2", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd_1, nullptr, hInstance, nullptr);

	windows[hWnd_1] = WindowDescription(hWnd_1);
	windows[hWnd_2] = WindowDescription(hWnd_2);

	if (!hWnd_1 || !hWnd_2)
	{
		return FALSE;
	}

	ShowWindow(hWnd_1, nCmdShow);
	UpdateWindow(hWnd_1);

	ShowWindow(hWnd_2, nCmdShow);
	UpdateWindow(hWnd_2);
	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		GET
		windows[hWnd].set_coords(0, 0);
		break;
	case WM_LBUTTONDOWN:
		windows[hWnd].isChecked = !windows[hWnd].isChecked;
		InvalidateRect(hWnd, &(windows[hWnd].winRect), false);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		
		windows[hWnd].paint(hdc);

		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

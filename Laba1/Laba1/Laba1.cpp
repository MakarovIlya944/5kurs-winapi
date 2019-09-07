// Laba1.cpp : Определяет точку входа для приложения.
//

#include "Laba1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
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

HMENU CreateMyMenu(HWND hwnd)
{
	int offset = windows[hwnd].get_id() * 2;
	windows[hwnd].one_event = 1001 + offset;
	windows[hwnd].both_event = 1002 + offset;

	HMENU hMainMenu = CreateMenu();
	HMENU hPopMenuFile = CreatePopupMenu();

	AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuFile, L"Настройки");
	{
		AppendMenu(hPopMenuFile, MF_STRING | MF_UNCHECKED, windows[hwnd].one_event, L"Включить только в этом окне");
		AppendMenu(hPopMenuFile, MF_STRING | MF_CHECKED, windows[hwnd].both_event, L"Включить в обоих окнах");
		AppendMenu(hPopMenuFile, MF_SEPARATOR, 1000, L"");
		AppendMenu(hPopMenuFile, MF_STRING, IDM_EXIT, L"Выход");
	}
	AppendMenu(hMainMenu, MF_STRING, IDM_ABOUT, L"Справка");

	windows[hwnd].one_id = GetMenuItemID(hPopMenuFile, 0);
	windows[hwnd].both_id = GetMenuItemID(hPopMenuFile, 1);

	MENUINFO mi;
	mi.cbSize = sizeof(mi);
	mi.hbrBack = CreateSolidBrush(RGB(200, 50, 50));
	mi.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;
	mi.dwStyle = 0;

	SetMenuInfo(hMainMenu, &mi);
	SetMenuInfo(hPopMenuFile, &mi);

	return hMainMenu;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
	HWND hWnd_1 = CreateWindowW(szWindowClass, L"Title 1", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	HWND hWnd_2 = CreateWindowW(szWindowClass, L"Title 2", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hWnd_1, nullptr, hInstance, nullptr);

	windows[hWnd_1] = WindowDescription(hWnd_1);
	windows[hWnd_2] = WindowDescription(hWnd_2);
	SetMenu(hWnd_1, CreateMyMenu(hWnd_1));
	SetMenu(hWnd_2, CreateMyMenu(hWnd_2));

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		windows[hWnd].set_coords(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		InvalidateRect(hWnd, &(windows[hWnd].winRect), false);
		break;
	case WM_LBUTTONDOWN:
		windows[hWnd].isChecked = !windows[hWnd].isChecked;
		InvalidateRect(hWnd, &(windows[hWnd].winRect), false);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		if (wmId > 1000)
		{
			if (windows[hWnd].one_event == wmId)
			{
				for (auto iter = windows.begin(), end = windows.end(); iter != end; iter++)
					if (iter->first != hWnd)
					{
						HMENU menu = GetMenu(iter->first);
						MENUINFO easd;
						GetMenuInfo(menu, &easd);
						MENUITEMINFO info;
						GetMenuItemInfo(menu, iter->second.one_id, false, &info);
						auto v = info.hbmpChecked;
						auto c = info.hbmpUnchecked;
						int a = 4;
					}
			}
			else if (windows[hWnd].both_event == wmId);
		}
		else
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

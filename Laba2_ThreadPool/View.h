#include <windows.h>
#include "resource1.h"

void SetIcon(HWND hwnd)
{
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, 1, (LPARAM)hIcon);
}

void SetStringComboBox(HWND hwnd, bool type)
{
	if (type == 0)
	{
		SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"1.Высокий");
		SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"2.Средний");
		SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"3.Низкий");
	}
	else
	{
		SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"1.Высокая");
		SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"2.Средняя");
		SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)"3.Низкая");
	}
	SendMessage(hwnd, CB_SETCURSEL, 0, 0);
}
#include "stdafx.h"
#include "l14.h"
#include "RService.h"
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HANDLE thread;
ATOM                RegClass(HINSTANCE hInst);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WindowsProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void setControls(HWND hWnd);
void onBtnSearchClick(HWND hWnd);
void onBtnStopClick(HWND hWnd);
void listClear(HWND hList);
DWORD WINAPI searchThreadProc(CONST LPVOID lpParam);
std::vector<HKEY> getSearchKeys(HWND hWnd);

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
	hInst = hInst;
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 600, 550, nullptr, nullptr, hInst, nullptr);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void onBtnSearchClick(HWND hWnd) {
	onBtnStopClick(hWnd);
	thread = CreateThread(NULL, 0, &searchThreadProc, hWnd, 0, NULL);
	ResumeThread(thread);
}

void onBtnStopClick(HWND hWnd) {
	EnableWindow(GetDlgItem(hWnd, ID_BTN_CANCEL), false);
	SuspendThread(thread);
	TerminateThread(thread, 0);
}

void listClear(HWND hList) {
	SendMessage(hList, LB_RESETCONTENT, NULL, NULL);
}

DWORD WINAPI searchThreadProc(CONST LPVOID lpParam) {
	TCHAR buffer[1024];
	HWND hWnd = static_cast<HWND>(lpParam);
	HWND hList = GetDlgItem(hWnd, ID_LIST_RESULTS);
	listClear(hList);
	EnableWindow(GetDlgItem(hWnd, ID_BTN_CANCEL), true);
	GetWindowText(GetDlgItem(hWnd, ID_EDIT_QUERY), buffer, 1024);
	auto query = std::wstring(buffer);
	auto searchKeys = getSearchKeys(hWnd);
	RegistryService* regService = new RegistryService(hList);
	regService->search(query, searchKeys);
	EnableWindow(GetDlgItem(hWnd, ID_BTN_CANCEL), false);

	return 0;
}

std::vector<HKEY> getSearchKeys(HWND hWnd) {
	std::vector<HKEY> res;
	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKLM))
		res.push_back(HKEY_LOCAL_MACHINE);
	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKCC))
		res.push_back(HKEY_CURRENT_CONFIG);
	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKCU))
		res.push_back(HKEY_CURRENT_USER);
	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKCR))
		res.push_back(HKEY_CLASSES_ROOT);
	if (IsDlgButtonChecked(hWnd, ID_CHECK_HKU))
		res.push_back(HKEY_USERS);

	return res;
}

LRESULT CALLBACK WindowsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		setControls(hWnd);

		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BTN_SEARCH:
			onBtnSearchClick(hWnd);
			break;
		case ID_BTN_CANCEL:
			onBtnStopClick(hWnd);
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
	if (message == WM_INITDIALOG)
		return (INT_PTR)TRUE;
	if (message == WM_COMMAND)
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	return (INT_PTR)FALSE;
}

void setControls(HWND hWnd) {
	CreateWindow(L"edit", L"", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE, 20, 20, 200, 30, hWnd, (HMENU)ID_EDIT_QUERY, hInst, NULL);
	CreateWindow(L"button", L"Search", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 230, 20, 60, 30, hWnd, (HMENU)ID_BTN_SEARCH, hInst, NULL);
	CreateWindow(L"button", L"Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 20, 60, 30, hWnd, (HMENU)ID_BTN_CANCEL, hInst, NULL);
	EnableWindow(GetDlgItem(hWnd, ID_BTN_CANCEL), false);
	CreateWindow(L"listbox", L"Results", WS_CHILD | WS_VISIBLE | LBS_EXTENDEDSEL | WS_BORDER | WS_VSCROLL | WS_HSCROLL, 20, 70, 360, 400, hWnd, (HMENU)ID_LIST_RESULTS, hInst, NULL);
	SendDlgItemMessage(hWnd, ID_LIST_RESULTS, LB_SETHORIZONTALEXTENT, 1000, NULL);
	CreateWindow(L"button", L"HKLM", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 400, 70, 100, 50, hWnd, (HMENU)ID_CHECK_HKLM, hInst, NULL);
	CreateWindow(L"button", L"HKCU", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 400, 120, 100, 50, hWnd, (HMENU)ID_CHECK_HKCU, hInst, NULL);
	CreateWindow(L"button", L"HKCR", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 400, 170, 100, 50, hWnd, (HMENU)ID_CHECK_HKCR, hInst, NULL);
	CreateWindow(L"button", L"HKU", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 400, 220, 100, 50, hWnd, (HMENU)ID_CHECK_HKU, hInst, NULL);
	CreateWindow(L"button", L"HKCC", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 400, 270, 100, 50, hWnd, (HMENU)ID_CHECK_HKCC, hInst, NULL);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInst,
	_In_opt_ HINSTANCE hPInst,
	_In_ LPWSTR    lpCLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPInst);
	UNREFERENCED_PARAMETER(lpCLine);
	LoadStringW(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInst, IDC_LAB14, szWindowClass, MAX_LOADSTRING);
	RegClass(hInst);
	InitInstance(hInst, nCmdShow);
	HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_LAB14));
	MSG message;
	while (GetMessage(&message, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(message.hwnd, hAccelTable, &message))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	return (int)message.wParam;
}

ATOM RegClass(HINSTANCE hInst)
{
	WNDCLASSEXW wincex;
	wincex.cbSize = sizeof(WNDCLASSEX);
	wincex.style = CS_HREDRAW | CS_VREDRAW;
	wincex.lpfnWndProc = WindowsProc;
	wincex.cbClsExtra = 0;
	wincex.cbWndExtra = 0;
	wincex.hInstance = hInst;
	wincex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_LAB14));
	wincex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wincex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wincex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB14);
	wincex.lpszClassName = szWindowClass;
	wincex.hIconSm = LoadIcon(wincex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wincex.hbrBackground = CreateSolidBrush(RGB(230, 112, 77));
	return RegisterClassExW(&wincex);
}
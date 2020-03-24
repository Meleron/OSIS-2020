#include "stdafx.h"
#include "l12.h"
#define TIMER_ID 121
#define TIMER_INTERVAL 100

HINSTANCE hInst;
WCHAR title[100];
WCHAR winClass[100];
HANDLE t1;
HANDLE t2;
HANDLE t3;
HANDLE event;
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI threadProc(CONST LPVOID lpParam);

int margin = 0;
int state = 0;
void drawTriangle(HDC hdc, int x, int y, int r);
void drawFigure(HDC hdc);
void initializeThreads();
void startThreads();

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd = CreateWindowW(winClass, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE) {
		initializeThreads();
		event = CreateEvent(NULL, TRUE, FALSE, L"Event");
		SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
		return 0;
	}
	if (message == WM_COMMAND)
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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
	else if (message == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		SetEvent(event);
		drawFigure(hdc);
		ResetEvent(event);
		EndPaint(hWnd, &ps);
		return 0;
	}
	else if (message == WM_TIMER) {
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	}
	else if (message == WM_DESTROY) {
		CloseHandle(t1);
		CloseHandle(t2);
		CloseHandle(t3);
		CloseHandle(event);
		KillTimer(hWnd, NULL);
		PostQuitMessage(0);
		return 0;
	}
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
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

void drawFigure(HDC hdc) {
	HBRUSH brush1 = CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH brush2 = CreateSolidBrush(RGB(0, 255, 255));
	HBRUSH brush3 = CreateSolidBrush(RGB(230, 167, 179));
	POINT p[4] = { {100, 100}, {150, 50}, {200,100}, {150,150} };
	switch (state) {
	case 0:
		SelectObject(hdc, brush1);
		Rectangle(hdc, 100, 50, 200, 150);
		break;
	case 1:
		BeginPath(hdc);
		Polyline(hdc, p, 4);
		CloseFigure(hdc);
		EndPath(hdc);
		SelectObject(hdc, brush2);
		FillPath(hdc);
		break;
	case 2:
		SelectObject(hdc, brush3);
		Rectangle(hdc, 150, 100, 50, 0);
		break;
	}
}

void drawTriangle(HDC hdc, int x, int y, int r) {
	Ellipse(hdc, x - r, y - r, x + r, y + r);
}

void startThreads() {
	ResumeThread(t1);
	ResumeThread(t2);
	ResumeThread(t3);
}

void initializeThreads() {
	t1 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
	t2 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
	t3 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
}


DWORD WINAPI threadProc(CONST LPVOID lpParam) {
	while (true) {
		DWORD dwWaitResult = WaitForSingleObject(event, 1);
		switch (dwWaitResult) {
		case WAIT_OBJECT_0:
			state++;
			state %= 3;
			Sleep(150);
			break;

		case WAIT_ABANDONED:
			break;
		}
	}

	return 0;
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB12EVENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB12EVENT);
	wcex.lpszClassName = winClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hbrBackground = CreateSolidBrush(RGB(63, 230, 108));
	return RegisterClassExW(&wcex);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	LoadStringW(hInstance, IDS_APP_TITLE, title, 100);
	LoadStringW(hInstance, IDC_LAB12EVENT, winClass, 100);
	MyRegisterClass(hInstance);
	InitInstance(hInstance, nCmdShow);
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB12EVENT));
	MSG msg;
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
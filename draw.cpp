// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw.h"

#define MAX_LOADSTRING 100
#define TMR_1 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT value;

HWND hwndButton;

const INT window_width = 1200;						//Szerokoœæ okienka
const INT window_height = 700;						//Wysokoœæ okienka
const INT screen_refresh_rate = 75;				//Czêstotliwoœæ odœwierzania ekranu
INT X;
INT Y;

RECT drawArea1 = { 455, 15, 745, 625 };
RECT drawArea2;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void drawPeople(HDC hdc, INT X, INT Y);
void drawAll(HDC hdc, INT X, INT Y);

void MyOnPaint(HDC hdc)
{

	static bool move_right = TRUE;
	if (value < 0) {
		move_right = TRUE;
	}
	else if (value > 500) {
		move_right = FALSE;
	}
	if (move_right) {
		value += 2;
	}
	else {
		value -= 2;
	}
	Graphics graphics(hdc);
	Pen penBl(Color(255, 0, 0, 255), 3);
	//graphics.DrawLine(&pen,0,0,200,100);

	graphics.DrawRectangle(&penBl, 460, 20 + value, 280, 100);

	//Rysuje szyb windy
	Pen penB(Color(255, 0, 0, 0), 5);
	graphics.DrawLine(&penB, 450, 10, 750, 10);
	graphics.DrawLine(&penB, 450, 630, 750, 630);
	graphics.DrawLine(&penB, 450, 10, 450, 630);
	graphics.DrawLine(&penB, 750, 10, 750, 630);

	//Rysuje pietra
	Pen penC(Color(255, 0, 0, 0), 2);
	graphics.DrawLine(&penC, 100, 134, 450, 134);
	graphics.DrawLine(&penC, 750, 258, 1100, 258);
	graphics.DrawLine(&penC, 100, 382, 450, 382);
	graphics.DrawLine(&penC, 750, 506, 1100, 506);
	graphics.DrawLine(&penC, 100, 630, 450, 630);
	//drawAll(hdc, X, Y);

}

void drawPeople(HDC hdc, INT X, INT Y)
{
	Graphics graphics(hdc);

	Pen penC(Color(255, 0, 0, 0), 2);
	graphics.DrawEllipse(&penC, X-2, Y-87, 25, 25);	// head
	graphics.DrawLine(&penC, X+11, Y-60, X+11, Y-29);	// body
	graphics.DrawLine(&penC, X+11, Y-34, X, Y);  // legs
	graphics.DrawLine(&penC, X+11, Y-34, X+21, Y);
	graphics.DrawLine(&penC, X-6, Y-64, X+11, Y-39);  // arms
	graphics.DrawLine(&penC, X+27, Y-64, X+11,Y-39);
}

void drawAll(HDC hdc, INT X, INT Y)
{
	drawPeople(hdc, X = 150, Y = 134);
	drawPeople(hdc, X = 200, Y = 134);
	drawPeople(hdc, X = 250, Y = 134);
	drawPeople(hdc, X = 300, Y = 134);
	drawPeople(hdc, X = 100, Y = 258);
	drawPeople(hdc, X = 100, Y = 258);
	drawPeople(hdc, X = 100, Y = 258);
	drawPeople(hdc, X = 100, Y = 258);
	drawPeople(hdc, X = 100, Y = 382);
	drawPeople(hdc, X = 100, Y = 382);
	drawPeople(hdc, X = 100, Y = 382);
	drawPeople(hdc, X = 100, Y = 382);
	drawPeople(hdc, X = 100, Y = 506);
	drawPeople(hdc, X = 100, Y = 506);
	drawPeople(hdc, X = 100, Y = 506);
	drawPeople(hdc, X = 100, Y = 506);
	drawPeople(hdc, X = 100, Y = 630);
	drawPeople(hdc, X = 100, Y = 630);
	drawPeople(hdc, X = 100, Y = 630);
	drawPeople(hdc, X = 100, Y = 630);
}

void repaintWW(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea2)
{	
	InvalidateRect(hWnd, drawArea2, TRUE); //Narysuj2
	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
//	drawPeople(hdc);
	MyOnPaint(hdc);
	EndPaint(hWnd, &ps);
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); //Narysuj wszyskto
	else
		InvalidateRect(hWnd, drawArea, TRUE); //Narysuj drawArea
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc);
	EndPaint(hWnd, &ps);
}


int OnCreate(HWND window)
{
	SetTimer(window, TMR_1, 1000 / screen_refresh_rate, 0);		//Ustawia timer na czas trwania jednej klatki obrazu
	return 0;
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	value = 10;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, window_width, window_height, 0, NULL, NULL, hInstance, NULL);

	int posX = 10;
	int posY = 10;

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR4_3,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 30,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR4_2,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 60,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR4_1,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 90,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR4_0,
		hInstance,
		NULL);

	posY = 250;

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR2_4,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 30,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR2_3,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 60,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR2_1,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 90,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR2_1,
		hInstance,
		NULL);

	posY = 500;

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR0_4,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 30,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR0_3,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 60,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR0_2,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 90,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR0_1,
		hInstance,
		NULL);

	posX = 1140;
	posY = 150;

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR3_4,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 30,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR3_2,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 60,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR3_1,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 90,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR3_0,
		hInstance,
		NULL);

	posY = 400;

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR1_4,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 30,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR1_3,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 60,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR1_2,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		posX, posY + 90,
		30, 30,
		hWnd,
		(HMENU)ID_BUTTON_FLOOR1_0,
		hInstance,
		NULL);

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	//OnCreate(hWnd,wParam,lParam);
	//OnTimer(hWnd,wParam,lParam);
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_BUTTON_FLOOR0_1:

			break;
		case ID_BUTTON_FLOOR0_2:

			break;
		case ID_BUTTON_FLOOR0_3:

			break;
		case ID_BUTTON_FLOOR0_4:

			break;
		case ID_BUTTON_FLOOR1_0:

			break;
		case ID_BUTTON_FLOOR1_2:

			break;
		case ID_BUTTON_FLOOR1_3:

			break;
		case ID_BUTTON_FLOOR1_4:

			break;
		case ID_BUTTON_FLOOR2_0:

			break;
		case ID_BUTTON_FLOOR2_1:

			break;
		case ID_BUTTON_FLOOR2_3:

			break;
		case ID_BUTTON_FLOOR2_4:

			break;
		case ID_BUTTON_FLOOR3_0:

			break;
		case ID_BUTTON_FLOOR3_1:

			break;
		case ID_BUTTON_FLOOR3_2:

			break;
		case ID_BUTTON_FLOOR3_4:

			break;
		case ID_BUTTON_FLOOR4_0:

			break;
		case ID_BUTTON_FLOOR4_1:

			break;
		case ID_BUTTON_FLOOR4_2:

			break;
		case ID_BUTTON_FLOOR4_3:

			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		MyOnPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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

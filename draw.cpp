﻿// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw.h"
#include <vector>
#include <queue>

#define MAX_LOADSTRING 100
#define TMR_SYSTEM 1
#define TMR_LIFTUP 2
#define TMR_LIFTDOWN 3


 // Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT value;
INT liftFloor = 0;			//piętro na którym jest winda
INT liftAnimValue = 0;
INT floorHeight;
INT nextFloorHeight;
INT liftState = 0;			//Jeżeli == 0 to stoi winda, == 1 to jedzie do góry, == -1 jedzie na dół

HWND hwndButton;

const INT window_width = 1200;						//Szerokość okienka
const INT window_height = 700;						//Wysokość okienka
const INT screen_refresh_rate = 75;				//Czêstotliwość odœwierzania ekranu

struct guy {
	INT whereToGo;		//Dokąd jadę
};

INT guysInLift[5];					//Tablica ludzi będących w windzie

std::queue <guy> floor0up;			//Kolejki ludzi którzy jadą na poszczególnych piętrach na dół albo do góry
std::queue <guy> floor1up;
std::queue <guy> floor1down;
std::queue <guy> floor2up;
std::queue <guy> floor2down;
std::queue <guy> floor3up;
std::queue <guy> floor3down;
std::queue <guy> floor4down;

RECT drawArea1 = { 455, 15, 745, 625 };
RECT drawArea2 = { 440, 15, 770, 625 };

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


void liftSystem(HDC hdc, HWND hWnd, PAINTSTRUCT &ps);



void fillDoor(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea) {
	InvalidateRect(hWnd, drawArea, TRUE); //Narysuj drawArea
	hdc = BeginPaint(hWnd, &ps);
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, drawArea, brush);
	DeleteObject(brush);
	EndPaint(hWnd, &ps);
}

void openDoor(INT floor, HDC hdc, HWND hWnd, PAINTSTRUCT &ps) {			//Rysuje biały prostokąt iminutjący otwarte drzwi
	Graphics graphics(hdc);
	RECT rect;
	switch (floor)
	{
	case 0:
		rect = { 440, 526, 470, 619 };
		break;
	case 1:
		rect = { 730, 412, 760, 505 };
		break;
	case 2:
		rect = { 440, 288, 470, 381 };
		break;
	case 3:
		rect = { 730, 164, 760, 257 };
		break;
	case 4:
		rect = { 440, 40, 470, 133 };
		break;
	}
	fillDoor(hWnd, hdc, ps, &rect);
}


void closeDoor(INT floor, HDC hdc, HWND hWnd, PAINTSTRUCT &ps) {
	RECT rect;
	switch (floor)
	{
	case 0:
		rect = { 440, 526, 470, 619 };
		break;
	case 1:
		rect = { 730, 412, 760, 505 };
		break;
	case 2:
		rect = { 440, 288, 470, 381 };
		break;
	case 3:
		rect = { 730, 164, 760, 257 };
		break;
	case 4:
		rect = { 440, 40, 470, 133 };
		break;
	}
	InvalidateRect(hWnd, &rect, TRUE); //Narysuj rect
	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Pen penB(Color(255, 0, 0, 0), 5);
	graphics.DrawLine(&penB, 450, 10, 450, 630);
	graphics.DrawLine(&penB, 750, 10, 750, 630);
	Pen penBl(Color(255, 0, 0, 255), 3);
	graphics.DrawLine(&penBl, 460, 10, 460, 630);
	graphics.DrawLine(&penBl, 740, 10, 740, 630);
	EndPaint(hWnd, &ps);
}

void openAndCloseDoor(INT floor, HDC hdc, HWND hWnd, PAINTSTRUCT &ps) {
	openDoor(floor, hdc, hWnd, ps);
	Sleep(500);
	closeDoor(floor, hdc, hWnd, ps);
}

void liftUp(HDC hdc, HWND window) {
	switch (liftFloor) 
	{
	case 4:
		break;
	case 3:
		SetTimer(window, TMR_LIFTUP, 1000 / screen_refresh_rate, 0);	//Ustawia timer na czas trwania jednej klatki obrazu
		break;
	case 2:
		SetTimer(window, TMR_LIFTUP, 1000 / screen_refresh_rate, 0);
		break;
	case 1:
		SetTimer(window, TMR_LIFTUP, 1000 / screen_refresh_rate, 0);
		break;
	case 0:
		SetTimer(window, TMR_LIFTUP, 1000 / screen_refresh_rate, 0);
		break;
	}
}

void liftDown(HDC hdc, HWND window) {
	switch (liftFloor)
	{
	case 4:
		SetTimer(window, TMR_LIFTDOWN, 1000 / screen_refresh_rate, 0);	//Ustawia timer na czas trwania jednej klatki obrazu
		break;
	case 3:
		SetTimer(window, TMR_LIFTDOWN, 1000 / screen_refresh_rate, 0);
		break;
	case 2:
		SetTimer(window, TMR_LIFTDOWN, 1000 / screen_refresh_rate, 0);
		break;
	case 1:
		SetTimer(window, TMR_LIFTDOWN, 1000 / screen_refresh_rate, 0);
		break;
	case 0:
		break;
	}
}


void MyOnPaint(HDC hdc)
{
	/*
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
	*/
	Graphics graphics(hdc);
	Pen penBl(Color(255, 0, 0, 255), 3);
	//graphics.DrawLine(&pen,0,0,200,100);
	value = 15;
	graphics.DrawRectangle(&penBl, 460, 520, 280, 100);

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
	graphics.DrawLine(&penC, 100, 620, 450, 620);

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

void animLiftUp(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	InvalidateRect(hWnd, drawArea, TRUE); //Narysuj drawArea
	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Pen penBl(Color(255, 0, 0, 255), 3);
	switch(liftFloor)
	{
	case 3:
		floorHeight = 258;
		nextFloorHeight = 134;
		break;
	case 2:
		floorHeight = 382;
		nextFloorHeight = 258;
		break;
	case 1:
		floorHeight = 506;
		nextFloorHeight = 382;
		break;
	case 0:
		floorHeight = 620;
		nextFloorHeight = 506;
		break;
	}
	graphics.DrawRectangle(&penBl, 460, floorHeight - liftAnimValue - 100, 280, 100);		//TODO tutaj jest malowana pusta winda
	liftAnimValue++;
	if (floorHeight - liftAnimValue <= nextFloorHeight) {
		liftFloor++;
		KillTimer(hWnd, TMR_LIFTUP);
		liftAnimValue = 0;
		liftSystem(hdc, hWnd, ps);
	}
	EndPaint(hWnd, &ps);
}

void animLiftDown(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	InvalidateRect(hWnd, drawArea, TRUE); //Narysuj drawArea
	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	Pen penBl(Color(255, 0, 0, 255), 3);
	switch (liftFloor)
	{
	case 4:
		floorHeight = 134;
		nextFloorHeight = 258;
		break;
	case 3:
		floorHeight = 258;
		nextFloorHeight = 382;
		break;
	case 2:
		floorHeight = 382;
		nextFloorHeight = 506;
		break;
	case 1:
		floorHeight = 506;
		nextFloorHeight = 620;
		break;
	}
	graphics.DrawRectangle(&penBl, 460, floorHeight + liftAnimValue - 100, 280, 100);		//TODO tutaj też jest malowana pusta winda
	liftAnimValue++;
	if (floorHeight + liftAnimValue >= nextFloorHeight) {
		liftFloor--;
		KillTimer(hWnd, TMR_LIFTDOWN);
		liftAnimValue = 0;
		liftSystem(hdc, hWnd, ps);
	}
	EndPaint(hWnd, &ps);
}


void liftSystem(HDC hdc, HWND hWnd, PAINTSTRUCT &ps) {
	switch (liftState)
	{
	case 0:		//gdy stoi w miejscu (implikuje to sytuację gdy jest pusta)
		if (!floor0up.empty()) {
			if (liftFloor != 0) {
				liftDown(hdc, hWnd);
				liftState = -1;
			}
			else {
				openAndCloseDoor(0, hdc, hWnd, ps);
				do {
					guysInLift[floor0up.front().whereToGo]++;
					floor0up.pop();
				} while (!floor0up.empty());		//Pakuje ludzi do windy aż się kolejka skończy
				liftUp(hdc, hWnd);
				liftState = 1;
			}
		}
		else if (!floor1up.empty()) {
			if (liftFloor == 0) {
				liftUp(hdc, hWnd);
				liftState = 1;
			}
			else if (liftFloor > 1) {
				liftDown(hdc, hWnd);
				liftState = -1;
			}
			else {
				openAndCloseDoor(1, hdc, hWnd, ps);
				do {
					guysInLift[floor1up.front().whereToGo]++;
					floor1up.pop();
				} while (!floor1up.empty());
				liftUp(hdc, hWnd);
				liftState = 1;
			}
		}
		else if (!floor1down.empty()) {
			if (liftFloor == 0) {
				liftUp(hdc, hWnd);
				liftState = 1;
			}
			else if (liftFloor > 1) {
				liftDown(hdc, hWnd);
				liftState = -1;
			}
			else {
				openAndCloseDoor(1, hdc, hWnd, ps);
				do {
					guysInLift[floor1down.front().whereToGo]++;
					floor1down.pop();
				} while (!floor1down.empty());
				liftDown(hdc, hWnd);
				liftState = -1;
			}
		}
		else if (!floor2up.empty()) {
			if (liftFloor < 2) {
				liftUp(hdc, hWnd);
				liftState = 1;
			}
			else if (liftFloor > 2) {
				liftDown(hdc, hWnd);
				liftState = -1;
			}
			else {
				openAndCloseDoor(2, hdc, hWnd, ps);
				do {
					guysInLift[floor2up.front().whereToGo]++;
					floor2up.pop();
				} while (!floor2up.empty());
				liftUp(hdc, hWnd);
				liftState = 1;
			}
		}
		else if (!floor2down.empty()) {
			if (liftFloor < 2) {
				liftUp(hdc, hWnd);
				liftState = 1;
			}
			else if (liftFloor > 2) {
				liftDown(hdc, hWnd);
				liftState = -1;
			}
			else {
				openAndCloseDoor(2, hdc, hWnd, ps);
				do {
					guysInLift[floor2down.front().whereToGo]++;
					floor2down.pop();
				} while (!floor2down.empty());
				liftDown(hdc, hWnd);
				liftState = -1;
			}
		}
		else if (!floor3up.empty()) {
			if (liftFloor < 3) {
				liftUp(hdc, hWnd);
				liftState = 1;
			}
			else if (liftFloor == 4) {
				liftDown(hdc, hWnd);
				liftState = -1;
			}
			else {
				openAndCloseDoor(3, hdc, hWnd, ps);
				do {
					guysInLift[floor3up.front().whereToGo]++;
					floor3up.pop();
				} while (!floor3up.empty());
				liftUp(hdc, hWnd);
				liftState = 1;
			}
		}
		else if (!floor3down.empty()) {
			if (liftFloor < 3) {
				liftUp(hdc, hWnd);
				liftState = 1;
			}
			else if (liftFloor == 4) {
				liftDown(hdc, hWnd);
				liftState = -1;
			}
			else {
				openAndCloseDoor(3, hdc, hWnd, ps);
				do {
					guysInLift[floor3down.front().whereToGo]++;
					floor3down.pop();
				} while (!floor3down.empty());
				liftDown(hdc, hWnd);
				liftState = -1;
			}
		}
		else if (!floor4down.empty()) {
			if (liftFloor != 4) {
				liftUp(hdc, hWnd);
				liftState = 1;
			}
			else {
				openAndCloseDoor(4, hdc, hWnd, ps);
				do {
					guysInLift[floor4down.front().whereToGo]++;
					floor4down.pop();
				} while (!floor4down.empty());
				liftDown(hdc, hWnd);
				liftState = -1;
			}
		}
		break;

		case 1:
			if (liftAnimValue == 0) {			//liftAnimValue == 0 gdy winda znajduje się na równo z jakimś piętrem
				if (liftFloor == 0) {
					if (!floor0up.empty() || (guysInLift[liftFloor] != 0)) {
						openAndCloseDoor(0, hdc, hWnd, ps);
					}
					while (!floor0up.empty()) {
						guysInLift[floor0up.front().whereToGo]++;
						floor1up.pop();
					}
					guysInLift[liftFloor] = 0;
				}
				if (liftFloor == 1) {
					if (!floor1up.empty() || (guysInLift[liftFloor] != 0)) {
						openAndCloseDoor(1, hdc, hWnd, ps);
					}
					while (!floor1up.empty()) {
						guysInLift[floor1up.front().whereToGo]++;
						floor1up.pop();
					}
					guysInLift[liftFloor] = 0;
				}
				if (liftFloor == 2) {
					if (!floor2up.empty() || (guysInLift[liftFloor] != 0)) {
						openAndCloseDoor(2, hdc, hWnd, ps);
					}
					while (!floor2up.empty()) {
						guysInLift[floor2up.front().whereToGo]++;
						floor2up.pop();
					}
					guysInLift[liftFloor] = 0;
				}
				if (liftFloor == 3) {
					if (!floor3up.empty() || (guysInLift[liftFloor] != 0)) {
						openAndCloseDoor(3, hdc, hWnd, ps);
					}
					while (!floor3up.empty()) {
						guysInLift[floor3up.front().whereToGo]++;
						floor3up.pop();
					}
					guysInLift[liftFloor] = 0;
				}
				if (liftFloor == 4) {
					if (guysInLift[liftFloor] != 0) {
						openAndCloseDoor(4, hdc, hWnd, ps);
					}
					guysInLift[liftFloor] = 0;
				}
				if (guysInLift[0] + guysInLift[1] + guysInLift[2] + guysInLift[3] + guysInLift[4] == 0) {
					liftState = 0;
				}
				else {
					liftUp(hdc, hWnd);
				}
			}
			break;

		case -1:
			if (liftAnimValue == 0) {
				if (liftFloor == 4) {
					if (!floor4down.empty() || (guysInLift[liftFloor] != 0)) {
						openAndCloseDoor(4, hdc, hWnd, ps);
					}
					while (!floor4down.empty()) {
						guysInLift[floor4down.front().whereToGo]++;
						floor4down.pop();
					}
					guysInLift[liftFloor] = 0;
				}
				if (liftFloor == 3) {
					if (!floor3down.empty() || (guysInLift[liftFloor] != 0)) {
						openAndCloseDoor(3, hdc, hWnd, ps);
					}
					while (!floor3down.empty()) {
						guysInLift[floor3down.front().whereToGo]++;
						floor3down.pop();
					}
					guysInLift[liftFloor] = 0;
				}
				if (liftFloor == 2) {
					if (!floor2down.empty() || (guysInLift[liftFloor] != 0)) {
						openAndCloseDoor(2, hdc, hWnd, ps);
					}
					while (!floor2down.empty()) {
						guysInLift[floor2down.front().whereToGo]++;
						floor2down.pop();
					}
					guysInLift[liftFloor] = 0;
				}
				if (liftFloor == 1) {
					if (!floor1down.empty() || (guysInLift[liftFloor] != 0)) {
						openAndCloseDoor(1, hdc, hWnd, ps);
					}
					while (!floor1down.empty()) {
						guysInLift[floor1down.front().whereToGo]++;
						floor1down.pop();
					}
					guysInLift[liftFloor] = 0;
				}
				if (liftFloor == 0) {
					if (guysInLift[liftFloor] != 0) {
						openAndCloseDoor(0, hdc, hWnd, ps);
					}
					guysInLift[liftFloor] = 0;
				}
				if (guysInLift[0] + guysInLift[1] + guysInLift[2] + guysInLift[3] + guysInLift[4] == 0) {
					liftState = 0;
				}
				else {
					liftDown(hdc, hWnd);
				}
			}
			break;
	}
}


int OnCreate(HWND window)
{
	SetTimer(window, TMR_SYSTEM, 1000 / screen_refresh_rate, 0);
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

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_BORDER,
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
		(HMENU)ID_BUTTON_FLOOR2_0,
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
	guy nowy;
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
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON_FLOOR0_1:
			nowy.whereToGo = 1;
			floor0up.push(nowy);
			break;
		case ID_BUTTON_FLOOR0_2:
			nowy.whereToGo = 2;
			floor0up.push(nowy);
			break;
		case ID_BUTTON_FLOOR0_3:
			nowy.whereToGo = 3;
			floor0up.push(nowy);
			break;
		case ID_BUTTON_FLOOR0_4:
			nowy.whereToGo = 4;
			floor0up.push(nowy);
			break;
		case ID_BUTTON_FLOOR1_0:
			nowy.whereToGo = 0;
			floor1down.push(nowy);
			break;
		case ID_BUTTON_FLOOR1_2:
			nowy.whereToGo = 2;
			floor1up.push(nowy);
			break;
		case ID_BUTTON_FLOOR1_3:
			nowy.whereToGo = 3;
			floor1up.push(nowy);
			break;
		case ID_BUTTON_FLOOR1_4:
			nowy.whereToGo = 4;
			floor1up.push(nowy);
			break;
		case ID_BUTTON_FLOOR2_0:
			nowy.whereToGo = 0;
			floor2down.push(nowy);
			break;
		case ID_BUTTON_FLOOR2_1:
			nowy.whereToGo = 1;
			floor2down.push(nowy);
			break;
		case ID_BUTTON_FLOOR2_3:
			nowy.whereToGo = 3;
			floor2up.push(nowy);
			break;
		case ID_BUTTON_FLOOR2_4:
			nowy.whereToGo = 4;
			floor2up.push(nowy);
			break;
		case ID_BUTTON_FLOOR3_0:
			nowy.whereToGo = 0;
			floor3down.push(nowy);
			break;
		case ID_BUTTON_FLOOR3_1:
			nowy.whereToGo = 1;
			floor3down.push(nowy);
			break;
		case ID_BUTTON_FLOOR3_2:
			nowy.whereToGo = 2;
			floor3down.push(nowy);
			break;
		case ID_BUTTON_FLOOR3_4:
			nowy.whereToGo = 4;
			floor3up.push(nowy);
			break;
		case ID_BUTTON_FLOOR4_0:
			nowy.whereToGo = 0;
			floor4down.push(nowy);
			break;
		case ID_BUTTON_FLOOR4_1:
			nowy.whereToGo = 1;
			floor4down.push(nowy);
			break;
		case ID_BUTTON_FLOOR4_2:
			nowy.whereToGo = 2;
			floor4down.push(nowy);
			break;
		case ID_BUTTON_FLOOR4_3:
			nowy.whereToGo = 3;
			floor4down.push(nowy);
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
		case TMR_SYSTEM:
			//repaintWindow(hWnd, hdc, ps, &drawArea1);
			hdc = BeginPaint(hWnd, &ps);
			if (liftState == 0) {
				liftSystem(hdc, hWnd, ps);
			}
			EndPaint(hWnd, &ps);
			break;
		case TMR_LIFTUP:
			animLiftUp(hWnd, hdc, ps, &drawArea1);
			break;
		case TMR_LIFTDOWN:
			animLiftDown(hWnd, hdc, ps, &drawArea1);
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
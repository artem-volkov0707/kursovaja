#include <winsock2.h>//библиотека для передачи данных по сети
#include <windows.h>
#include <process.h>//многопоточность....наверное
#include <tchar.h>
#include <math.h>//мат. действия

#pragma comment(lib,"WS2_32.lib")

// Global variables
static bool Hod;
static bool Sending=false;
static char A[8][8] = {/////0-Пусто    1-Чёрные    2-Белые
	{ 0, 2, 0, 2, 0, 2, 0, 2 },
	{ 2, 0, 2, 0, 2, 0, 2, 0 },
	{ 0, 2, 0, 2, 0, 2, 0, 2 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 0, 1, 0, 1, 0, 1, 0 },
	{ 0, 1, 0, 1, 0, 1, 0, 1 },
	{ 1, 0, 1, 0, 1, 0, 1, 0 }
};
static char Pusk[7];
// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		750, 670,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//



WORD sockVer;
WSADATA wsaData;
int retVal;
SOCKET servSock;
SOCKET clientSock;
//void Server(int vers){
void Server(int vers){
	
	if (vers == 0){
		sockVer = MAKEWORD(2, 2);

		WSAStartup(sockVer, &wsaData);

		//Создаем сокет
	//	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (servSock == INVALID_SOCKET)
		{
			WSACleanup();
			return;
		}
		SOCKADDR_IN sin;
		sin.sin_family = PF_INET;
		sin.sin_port = htons(1111);
		sin.sin_addr.s_addr = INADDR_ANY;

		retVal = bind(servSock, (LPSOCKADDR)&sin, sizeof(sin));
		if (retVal == SOCKET_ERROR)
		{
			WSACleanup();
			return;
		}

		//Пытаемся начать слушать сокет
		retVal = listen(servSock, 10);
		if (retVal == SOCKET_ERROR)
		{
			WSACleanup();
			return;
		}
		clientSock = accept(servSock, NULL, NULL);

		if (clientSock == INVALID_SOCKET)
		{

			WSACleanup();
			return;
		}
	}
	if (vers == 1){//отправляем
		if (SOCKET_ERROR == (retVal = send(clientSock, Pusk, sizeof (Pusk), 0)))
		{
			// Error
			int res = WSAGetLastError();
			return;
		}

		Sending = true;
	}
	if (vers == 2){//принимаем
		char            buff[7];
		retVal = recv(clientSock, (char *)&buff, 7, 0);
		if (retVal == SOCKET_ERROR)
			{
				WSACleanup();
				return;
			}
		int x, y;
		y = buff[0];
		x = buff[1];
		A[y][x] = 0;
		y = buff[2];
		x = buff[3];
		A[y][x] = 2;
		if (buff[4] != 'r'){
			y = buff[4];
			x = buff[5];
			A[y][x] = 0;
		}			
		Hod = true;
	}
	return;
}


WORD ver = MAKEWORD(2, 2);
LPHOSTENT hostEnt;
void Client(int vers){
	if (vers == 0){
		retVal = 0;
		WSAStartup(ver, (LPWSADATA)&wsaData);

		//	LPHOSTENT hostEnt;

		hostEnt = gethostbyname("localhost");

		if (!hostEnt)
		{
			WSACleanup();
			return;
		}

		//Создаем сокет
		//	SOCKET clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientSock == SOCKET_ERROR)
		{
			WSACleanup();
			return;
		}

		SOCKADDR_IN serverInfo;

		serverInfo.sin_family = PF_INET;
		serverInfo.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
		serverInfo.sin_port = htons(1111);

		retVal = connect(clientSock, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
		if (retVal == SOCKET_ERROR)
		{
			WSACleanup();
			return;
		}

	}
	if (vers == 1) {	//отправляем	
		if (SOCKET_ERROR == (retVal = send(clientSock, Pusk, sizeof (Pusk), 0)))
		{
			// Error
			int res = WSAGetLastError();
			return;
		}
		Hod = false;
		Sending = true;
	}
	if (vers == 2) {
			char szResponse[9];
			char            buff[7];
			retVal = recv(clientSock, (char *)&buff,7, 0);
			if (retVal == SOCKET_ERROR)
			{
				WSACleanup();
				return;
			}
			int x, y;
			y = buff[0];
			x = buff[1];
			A[y][x] = 0;
			y = buff[2];
			x = buff[3];
			A[y][x] = 1;
			if (buff[4] != 'r'){
				y = buff[4];
				x = buff[5];
				A[y][x] = 0;
			}
			Hod = true;
	}
	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	HDC hdc, hCompatibleDC;
	PAINTSTRUCT PaintStruct;
	HANDLE hBitmap, hOldBitmap;
	BITMAP Bitmap;
	static HWND hWndButton[2];
	static int z = 0;
	double xx, yy;
	static bool  V = true, Serv = true, flag,ok=false;//true-Ход чёрных, false-Ход белых
	static int cxblock = 75, cyblock = 75;
	static int k, m, f = 0, ff = 0;
	static int zz = 0;
	static CRITICAL_SECTION cs;

	InitializeCriticalSection(&cs);

	switch (message)
	{
	case WM_CREATE:
		hWndButton[0] = CreateWindow(_T("BUTTON"), _T("Создать сервер"),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			600, 0,
			130, 30,
			hWnd, (HMENU)1,
			hInst, 0);
		hWndButton[1] = CreateWindow(_T("BUTTON"), _T("Соединение"),
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			600, 31,
			130, 30,
			hWnd, (HMENU)2,
			hInst, 0);
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case 1:
			Server(0);
			Hod = true;
			Serv = true;
			InvalidateRect(hWnd, 0, true);

			break;
		case 2:
			Client(0);
			InvalidateRect(hWnd, 0, true);
			Hod = false;
			Serv = false;
			Client(2);
			break;
		}
		break;
	case WM_LBUTTONUP:
		if (Hod == true){
			if (Serv == true){
				if (V == true){
					xx = LOWORD(lParam) / cxblock;
					yy = HIWORD(lParam) / cyblock;
					xx = floor(xx);
					yy = floor(yy);
					if (xx < 8 && yy < 8){
						k = (int)xx;
						m = (int)yy;
						if (A[m][k] == 1){
							xx = xx*cxblock;
							yy = yy*cyblock;
							hdc = GetDC(hWnd);
							hBitmap = LoadImage(NULL, L"black3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
							GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
							hCompatibleDC = CreateCompatibleDC(hdc);
							hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
							StretchBlt(hdc, xx, yy, 60, 60, hCompatibleDC, 0, 0, Bitmap.bmWidth,
								Bitmap.bmHeight, SRCCOPY);
							SelectObject(hCompatibleDC, hOldBitmap);
							DeleteObject(hBitmap);
							DeleteDC(hCompatibleDC);
							ReleaseDC(hWnd, hdc);
							flag = true;
						}
					}
				}
				if (V == false){
					int x, y;
					x = LOWORD(lParam) / cxblock;
					y = HIWORD(lParam) / cyblock;
					x = floor((double)x);
					y = floor((double)y);
					V = true;
					flag = false;
					if (x < 8 && y < 8){
						if (A[y][x] == 0 && ( (((x - 1) == k) && ((y + 1) == m)) ||
							 (((x + 1) == k) && ((y + 1) == m)))){
							A[y][x] = 1;
							A[m][k] = 0;
							ok = true;
							Hod = false;
							Pusk[0] = m;
							Pusk[1] = k;
							Pusk[2] = y;
							Pusk[3] = x;
							Pusk[4] = 'e';
							InvalidateRect(hWnd, 0, true);
						}
						if (A[y][x] == 0 && ((((x - 2) == k) && ((y - 2) == m)) || (((x - 2) == k) && ((y + 2) == m)) ||
							(((x + 2) == k) && ((y - 2) == m)) || (((x + 2) == k) && ((y + 2) == m)))){
							if (A[(m + y) / 2][(k + x) / 2] == 2){
								A[(m + y) / 2][(k + x) / 2] = 0;
								A[y][x] = 1;
								A[m][k] = 0;
								Pusk[0] = m;
								Pusk[1] = k;
								Pusk[2] = y;
								Pusk[3] = x;
								Pusk[4] = (m + y) / 2;
								Pusk[5] = (k + x) / 2;
								ok = true;
								Hod = false;
								InvalidateRect(hWnd, 0, true);
							}
						}
						if (x == k&&y == m){
							InvalidateRect(hWnd, 0, true);
						}
			//			InvalidateRect(hWnd, 0, true);
					}
				}
				if (flag == true){
					V = false;
				}
				if (flag == false){
					V = true;
				}
			/*	if (ok == true){
					Server(1, true, hWnd);
					InvalidateRect(hWnd, 0, true);
					Server(1, false, hWnd);
			//		InvalidateRect(hWnd, 0, true);
					ok = false;
				}*/
			}
			if (Serv == false){
				if (V == true){
					xx = LOWORD(lParam) / cxblock;
					yy = HIWORD(lParam) / cyblock;
					xx = floor(xx);
					yy = floor(yy);
					if (xx < 8 && yy < 8){
						k = (int)xx;
						m = (int)yy;
						if (A[m][k] == 2){
							xx = xx*cxblock;
							yy = yy*cyblock;
							hdc = GetDC(hWnd);
							hBitmap = LoadImage(NULL, L"red1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
							GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
							hCompatibleDC = CreateCompatibleDC(hdc);
							hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
							//GetClientRect(hWnd, &Rect);
							StretchBlt(hdc, xx, yy, 60, 60, hCompatibleDC, 0, 0, Bitmap.bmWidth,
								Bitmap.bmHeight, SRCCOPY);
							SelectObject(hCompatibleDC, hOldBitmap);
							DeleteObject(hBitmap);
							DeleteDC(hCompatibleDC);
							ReleaseDC(hWnd, hdc);
							flag = true;
						}
					}
				}
				if (V == false){
					int x, y;
					x = LOWORD(lParam) / cxblock;
					y = HIWORD(lParam) / cyblock;
					x = floor((double)x);
					y = floor((double)y);
					V = true;
					flag = false;
					if (x < 8 && y < 8){
						if (A[y][x] == 0 && ((((x - 1) == k) && ((y - 1) == m))  ||
							(((x + 1) == k) && ((y - 1) == m)) )){
							A[y][x] = 2;
							A[m][k] = 0;
							Pusk[0] = m;
							Pusk[1] = k;
							Pusk[2] = y;
							Pusk[3] = x;
							Pusk[4] = 'r';
							ok = true;
							Hod = false;
							InvalidateRect(hWnd, 0, true);
						}
						if (A[y][x] == 0 && ((((x - 2) == k) && ((y - 2) == m)) || (((x - 2) == k) && ((y + 2) == m)) ||
							(((x + 2) == k) && ((y - 2) == m)) || (((x + 2) == k) && ((y + 2) == m)))){
							if (A[(m + y) / 2][(k + x) / 2] == 1){
								A[(m + y) / 2][(k + x) / 2] = 0;
								A[y][x] = 2;
								A[m][k] = 0;
								Pusk[0] = m;
								Pusk[1] = k;
								Pusk[2] = y;
								Pusk[3] = x;
								Pusk[4] = (m + y) / 2;
								Pusk[5] = (k + x) / 2;
								ok = true;
								Hod = false;
								InvalidateRect(hWnd, 0, true);
							}
						}
						if (x == k&&y == m){
							InvalidateRect(hWnd, 0, true);
						}
					}
				}
				if (flag == true){
					V = false;
				}
				if (flag == false){
					V = true;
				}
			}
		}
		break;

	case WM_PAINT:

		
		hdc = BeginPaint(hWnd, &PaintStruct);
		hBitmap = LoadImage(NULL, L"picture1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		hCompatibleDC = CreateCompatibleDC(hdc);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
		StretchBlt(hdc, 0, 0, 600, 600, hCompatibleDC, 0, 0, Bitmap.bmWidth,
			Bitmap.bmHeight, SRCCOPY);
		SelectObject(hCompatibleDC, hOldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(hCompatibleDC);
		if (Serv == true && ok == true){
			Server(1);
		}
		if (Serv == false && ok == true){
			Client(1);
		}
		for (int ii = 0; ii < 8; ++ii){

			for (int jj = 0; jj < 8; ++jj){
					xx = jj*cxblock;
					yy = ii*cyblock;
					if (A[ii][jj] == 1){
						hBitmap = LoadImage(NULL, L"black2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					}
					if (A[ii][jj] == 2){
						hBitmap = LoadImage(NULL, L"red.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					}
					GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
					hCompatibleDC = CreateCompatibleDC(hdc);
					hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
					StretchBlt(hdc, xx, yy, 60, 60, hCompatibleDC, 0, 0, Bitmap.bmWidth,
						Bitmap.bmHeight, SRCCOPY);
					SelectObject(hCompatibleDC, hOldBitmap);
					DeleteObject(hBitmap);
					DeleteDC(hCompatibleDC);
				
			}
		}
		if (Serv == true && ok == true){
			Server(2);
			ok = false;
			InvalidateRect(hWnd, 0, true);
		}
		if (Serv == false && ok == true){
			Client(2);
			ok = false;
			InvalidateRect(hWnd, 0, true);
		}
		EndPaint(hWnd, &PaintStruct);
		
		break;
	case WM_DESTROY:
		closesocket(clientSock);
		closesocket(servSock);
		WSACleanup();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
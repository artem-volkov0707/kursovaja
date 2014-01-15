
#include <windows.h>
#include <math.h>

//#include <afxwin.h>
//#include <afxsock.h>
#include <richedit.h>
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"Gdi32.lib")
#pragma comment(lib,"LIBCMTD.lib")
// ���������� �������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

////////////////////////////////////////////////////////////////////////// 
// ������� ��������� ��������� WinMain
int APIENTRY tWinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)
{

	// ��� �������� ������
	LPCTSTR lpzClass = TEXT("My Window Class!");

	// ����������� ������
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	// ���������� ��������� ������ ������
	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect); // ���������� ������
	int x = screen_rect.right / 2 - 150;
	int y = screen_rect.bottom / 2 - 75;

	// �������� ����������� ����
	HWND hWnd = CreateWindow(lpzClass, TEXT("Dialog Window"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, 700, 640, NULL, NULL,
		hInstance, NULL);

	// ���� ���� �� �������, ��������� ����� ����� 0
	if (!hWnd) return 2;

	// ���� ��������� ����������
	MSG msg = { 0 };    // ��������� ���������
	int iGetOk = 0;   // ���������� ���������
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) // ���� ���������
	{
		if (iGetOk == -1) return 3;  // ���� GetMessage ������ ������ - �����
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;  // ���������� ��� ���������� ���������
}

////////////////////////////////////////////////////////////////////////// 
// ������� ����������� ������ ����
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	// ����� �-��� ��������� ���������
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	// ����� ����
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	// ���������� ���������� ����������
	wcWindowClass.hInstance = hInst;
	// �������� ������
	wcWindowClass.lpszClassName = lpzClassName;
	// �������� �������
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// �������� ����� ����
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	return RegisterClass(&wcWindowClass); // ����������� ������
}

////////////////////////////////////////////////////////////////////////// 
// ������� ��������� ���������
LRESULT CALLBACK WndProc(	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
	HDC hdc, hCompatibleDC;
	PAINTSTRUCT PaintStruct;
	HANDLE hBitmap, hOldBitmap;
//	RECT Rect;
	BITMAP Bitmap;
	static int A[8][8] = {/////0-�����    1-׸����    2-�����
		{ 0, 2, 0, 2, 0, 2, 0, 2 },
		{ 2, 0, 2, 0, 2, 0, 2, 0 },
		{ 0, 2, 0, 2, 0, 2, 0, 2 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 }
	};
	static int z = 0;
	double xx, yy;
	static bool Hod = true, V = true;//true-��� ������, false-��� �����
	static int cxblock = 75, cyblock = 75;
	static int k, m, f = 0, ff = 0;
	// ������� � ��������� ���������
	switch (message)
	{
	case WM_LBUTTONUP:


		if (V == true){
			xx = LOWORD(lParam) / cxblock;
			yy = HIWORD(lParam) / cyblock;
			xx = floor(xx);
			yy = floor(yy);
			if (xx < 8 && yy < 8){
				k = (int)xx;
				m = (int)yy;
				if (Hod == true){
					if (A[m][k] == 1){
						xx = xx*cxblock;
						yy = yy*cyblock;
						hdc = GetDC(hWnd);
						hBitmap = LoadImage(NULL, L"black3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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
						f = 1;
					}
				}

				if (Hod == false){
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
						f = 1;
					}
				}
			}
		}
		if (V == false){
			int x, y;
			x = LOWORD(lParam) / cxblock;
			y = HIWORD(lParam) / cyblock;
			x = floor((double)x);
			y = floor((double)y);
			if (x < 8 && y < 8){
				if (Hod == true){
					if (A[y][x] == 0 && ((((x - 1) == k) && ((y - 1) == m)) || (((x - 1) == k) && ((y + 1) == m)) ||
						(((x + 1) == k) && ((y - 1) == m)) || (((x + 1) == k) && ((y + 1) == m)))){
						A[y][x] = 1;
						A[m][k] = 0;
						f = 2;
						ff = 2;
					}
					if (A[y][x] == 0 && ((((x - 2) == k) && ((y - 2) == m)) || (((x - 2) == k) && ((y + 2) == m)) ||
						(((x + 2) == k) && ((y - 2) == m)) || (((x + 2) == k) && ((y + 2) == m)))){
						if (A[(m + y) / 2][(k + x) / 2] == 2){
							A[(m + y) / 2][(k + x) / 2] = 0;
							A[y][x] = 1;
							A[m][k] = 0;
							f = 2;
							ff = 2;
						}
					}
					if (x == k&&y == m){
						f = 2;
						ff = 1;
					}
					InvalidateRect(hWnd, 0, true);
				}
				if (Hod == false){
					if (A[y][x] == 0 && ((((x - 1) == k) && ((y - 1) == m)) || (((x - 1) == k) && ((y + 1) == m)) ||
						(((x + 1) == k) && ((y - 1) == m)) || (((x + 1) == k) && ((y + 1) == m)))){
						A[y][x] = 2;
						A[m][k] = 0;
						f = 2;
						ff = 1;
					}
					if (A[y][x] == 0 && ((((x - 2) == k) && ((y - 2) == m)) || (((x - 2) == k) && ((y + 2) == m)) ||
						(((x + 2) == k) && ((y - 2) == m)) || (((x + 2) == k) && ((y + 2) == m)))){
						if (A[(m + y) / 2][(k + x) / 2] == 1){
							A[(m + y) / 2][(k + x) / 2] = 0;
							A[y][x] = 2;
							A[m][k] = 0;
							f = 2;
							ff = 1;
						}
					}
					if (x == k&&y == m){
						f = 2;
						ff = 2;
					}
					InvalidateRect(hWnd, 0, true);
				}
			}
		}
		if (f == 1){
			V = false;
		}
		if (f == 2){
			V = true;
		}
		if (ff == 2){
			Hod = false;
		}
		if (ff == 1){
			Hod = true;
		}
		break;


		case WM_PAINT:
			hdc = BeginPaint(hWnd, &PaintStruct);

			hBitmap = LoadImage(NULL, L"picture1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
			hCompatibleDC = CreateCompatibleDC(hdc);
			hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
			//GetClientRect(hWnd, &Rect);
			StretchBlt(hdc, 0, 0, 600, 600, hCompatibleDC, 0, 0, Bitmap.bmWidth,
				Bitmap.bmHeight, SRCCOPY);
			SelectObject(hCompatibleDC, hOldBitmap);
			DeleteObject(hBitmap);
			DeleteDC(hCompatibleDC);
			for (int ii = 0; ii < 8; ++ii){
				for (int jj = 0; jj < 8; ++jj){
					if (A[ii][jj] == 1){
						xx = jj*cxblock;
						yy = ii*cyblock;
						hBitmap = LoadImage(NULL, L"black2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
						GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
						hCompatibleDC = CreateCompatibleDC(hdc);
						hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
						//GetClientRect(hWnd, &Rect);
						StretchBlt(hdc, xx, yy, 60, 60, hCompatibleDC, 0, 0, Bitmap.bmWidth,
							Bitmap.bmHeight, SRCCOPY);
						SelectObject(hCompatibleDC, hOldBitmap);
						DeleteObject(hBitmap);
						DeleteDC(hCompatibleDC);
					}
					if (A[ii][jj] == 2){
						xx = jj*cxblock;
						yy = ii*cyblock;
						hBitmap = LoadImage(NULL, L"red.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
						GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
						hCompatibleDC = CreateCompatibleDC(hdc);
						hOldBitmap = SelectObject(hCompatibleDC, hBitmap);
						//GetClientRect(hWnd, &Rect);
						StretchBlt(hdc, xx, yy, 60, 60, hCompatibleDC, 0, 0, Bitmap.bmWidth,
							Bitmap.bmHeight, SRCCOPY);
						SelectObject(hCompatibleDC, hOldBitmap);
						DeleteObject(hBitmap);
						DeleteDC(hCompatibleDC);
					}
				}
			}
			EndPaint(hWnd, &PaintStruct);
			break;
	case WM_DESTROY:
		PostQuitMessage(0);  // ������� �� ���������
		break;
	default:
		// ��� ��������� �� ������������ ���� ���������� ���� Windows
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
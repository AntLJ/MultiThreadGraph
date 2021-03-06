// MultiThreadGraph.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "MultiThreadGraph.h"
#include <process.h>
typedef struct
{
	HWND hwnd;
	int cxClient;
	int cyClient;
	int cyChar;
	BOOL bKill;
}
PARAMS, *PPARAMS;

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

HWND hWnd;
int cxClient, cyClient;

// このコード モジュールに含まれる関数の宣言を転送します:
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

	// TODO: ここにコードを挿入してください。

	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MULTITHREADGRAPH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーション初期化の実行:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MULTITHREADGRAPH));

	MSG msg;

	// メイン メッセージ ループ:
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

VOID Thread1(PVOID pvoid)
{
	HBRUSH hBrush;
	HDC hdc;
	int xLeft, xRight, yTop, yBottom, iRed, iGreen, iBlue;


	while (TRUE)
	{
		if (cxClient != 0 || cyClient != 0)
		{
			xLeft = rand() % (cxClient / 2);
			xRight = rand() % (cxClient / 2);
			yTop = rand() % cyClient;
			yBottom = rand() % cyClient;
			iRed = rand() & 255;
			iGreen = rand() & 255;
			iBlue = rand() & 255;
			hdc = GetDC(hWnd);
			hBrush = CreateSolidBrush(RGB(iRed, iGreen, iBlue));
			SelectObject(hdc, hBrush);

			Rectangle(hdc, min(xLeft, xRight), min(yTop, yBottom), max(xLeft, xRight), max(yTop, yBottom));

			ReleaseDC(hWnd, hdc);
			DeleteObject(hBrush);
			Sleep(100);
		}
	}
	_endthread();
}

VOID Thread2(PVOID pvoid)
{
	HBRUSH hBrush;
	HDC hdc;
	int xLeft2, xRight2, yTop2, yBottom2, iRed2, iGreen2, iBlue2;

	while (TRUE)
	{
		if (cxClient != 0 || cyClient != 0)
		{
			xLeft2 = rand() % (cxClient / 2) + (cxClient / 2);
			xRight2 = rand() % (cxClient / 2) + (cxClient / 2);
			yTop2 = rand() % cyClient;
			yBottom2 = rand() % cyClient;

			iRed2 = rand() & 255;
			iGreen2 = rand() & 255;
			iBlue2 = rand() & 255;

			hdc = GetDC(hWnd);
			hBrush = CreateSolidBrush(RGB(iRed2, iGreen2, iBlue2));
			SelectObject(hdc, hBrush);

			Ellipse(hdc, xLeft2, yTop2, xRight2, yBottom2);

			ReleaseDC(hWnd, hdc);
			DeleteObject(hBrush);
			Sleep(100);
		}
	}
	_endthread();
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MULTITHREADGRAPH));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MULTITHREADGRAPH);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	return TRUE;
}

LRESULT CALLBACK WndProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static PARAMS params;
	switch (message)
	{
	case WM_CREATE:
		params.hwnd = hWnd;
		//params.cyChar = HIWORD(GetDialogBaseUnits());
		_beginthread(Thread1, 0, NULL);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_DESTROY:
		params.bKill = TRUE;
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static PARAMS params;
	switch (message)
	{
	case WM_CREATE:
		params.hwnd = hWnd;
		//params.cyChar = HIWORD(GetDialogBaseUnits());
		_beginthread(Thread2, 0, NULL);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_DESTROY:
		params.bKill = TRUE;
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndChild[2];
	static const TCHAR * szChildClass[] = { TEXT("Child1"), TEXT("Child2") };
	static WNDPROC ChildProc[] = { WndProc1, WndProc2 };
	HINSTANCE hInstance;
	WNDCLASSEXW wcex;

	switch (message)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MULTITHREADGRAPH));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MULTITHREADGRAPH);
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		for (int i = 0; i < 2; i++)
		{
			wcex.lpfnWndProc = ChildProc[i];
			wcex.lpszClassName = szChildClass[i];

			RegisterClassExW(&wcex);

			hwndChild[i] = CreateWindowW(szChildClass[i], NULL,
				WS_CHILDWINDOW | WS_BORDER | WS_VISIBLE,
				0, 0, 0, 0,
				hWnd, (HMENU)i, hInstance, NULL);
		}

		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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

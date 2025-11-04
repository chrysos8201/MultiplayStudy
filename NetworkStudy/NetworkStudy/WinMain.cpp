#include "Core.h"
#include "GameProcess.h"
#include <windows.h>

#define MAX_LOADSTRING 100

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

HWND g_hWnd;
std::unique_ptr<GameProcess> gGameProcess;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void Initialize(HINSTANCE hInstance);
void Update();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Initialize(hInstance);
	Update();
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return 0;
}

void Initialize(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = hInstance;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"PingPong";

	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindow(
		L"PingPong",
		L"PingPong",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1135,
		955,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		nullptr);
	g_hWnd = hWnd;

	if (!hWnd)
	{
		return;
	}

	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::UpdateWindow(hWnd);

	// 그래픽 초기화
	//_d2d = new D2D();
	//_d2d->Initialize(_hwnd);
	//
	//// 게임 초기화
	//_game = new Game();
	//_game->Initialize(_d2d);
	gGameProcess = std::make_unique<GameProcess>();
	gGameProcess->Initialize(g_hWnd);
}

void Update()
{
	while (true)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			DispatchMessage(&msg);
		}
		else
		{
			gGameProcess->Update();
			gGameProcess->Render();
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return result;
}
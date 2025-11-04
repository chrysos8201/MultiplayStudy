#pragma once
#include <windows.h>

class GameProcess
{
public:
	void Initialize(HWND inHwnd);
	void Update();
	void Render();
	void ShutDown();
private:

private:
	HWND mHwnd;
	HDC mHdc; // 화면 DC
	HDC mHMemdc; // 백 버퍼 DC

	HBITMAP mHBitmap;		// 백 버퍼 비트맵
	HBITMAP mHOldBitmap;	// mHMemdc에 원래 선택되어 있던 비트맵
	RECT    mRect;			// 클라이언트 영역 크기
	HBRUSH  mHBackgroundBrush; // 배경색을 칠할 브러시
};


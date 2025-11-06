#include "GameProcess.h"
#include "ObjectCreationRegistry.h"
#include "LinkingContext.h"

void RegisterRegistry()
{
	ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<Player>();
}

void GameProcess::Initialize(HWND inhWnd)
{
	mHwnd = inhWnd;
	mHdc = GetDC(mHwnd); // 화면 DC를 얻어옴

	// 클라이언트 영역의 크기를 얻어와 mRect에 저장
	GetClientRect(mHwnd, &mRect);
	// 1. 백 버퍼(메모리 DC) 생성
	mHMemdc = CreateCompatibleDC(mHdc);
	// 2. 백 버퍼 비트맵 생성 (화면 DC와 호환되게)
	mHBitmap = CreateCompatibleBitmap(mHdc, mRect.right, mRect.bottom);
	// 3. 백 버퍼 DC에 비트맵을 선택하고, 원래 있던 비트맵을 저장
	mHOldBitmap = (HBITMAP)SelectObject(mHMemdc, mHBitmap);
	// 4. 요청한 배경색으로 브러시 생성
	mHBackgroundBrush = CreateSolidBrush(RGB(0,255,255));


	RegisterRegistry();


	mLinkingContext = std::make_shared<LinkingContext>();
	networkId0 = mLinkingContext->GetNetworkId(ObjectCreationRegistry::Get().CreateGameObject('PLER'), true);
	networkId1 = mLinkingContext->GetNetworkId(ObjectCreationRegistry::Get().CreateGameObject('PLER'), true);

	static_pointer_cast<Player>(mLinkingContext->GetGameObject(networkId0))->SetPosition(100, 100);
}

void GameProcess::Update()
{
	mLinkingContext->GetGameObject(networkId0)->Update();
	mLinkingContext->GetGameObject(networkId1)->Update();
}

void GameProcess::Render()
{
	// 1. 백 버퍼를 배경색으로 모두 지웁니다.
	FillRect(mHMemdc, &mRect, mHBackgroundBrush);

	// 2. 백 버퍼(mHMemdc)에 모든 게임 객체를 그립니다.
	//std::wstring message = L"This is Game";
	//TextOut(mHMemdc, 10, 10, message.c_str(), message.size());
	//Rectangle(mHMemdc, 50, 50, 100, 100);


	mLinkingContext->GetGameObject(networkId0)->Render(mHMemdc);
	mLinkingContext->GetGameObject(networkId1)->Render(mHMemdc);







	// 3. 백 버퍼에 그린 내용을 화면 DC(mHdc)로 한 번에 복사(BitBlt)합니다.
	BitBlt(mHdc, 0, 0, mRect.right, mRect.bottom, mHMemdc, 0, 0, SRCCOPY);
}

void GameProcess::ShutDown()
{
	SelectObject(mHMemdc, mHOldBitmap);
	DeleteObject(mHBackgroundBrush);
	DeleteObject(mHBitmap);
	DeleteDC(mHMemdc);
	ReleaseDC(mHwnd, mHdc);
}
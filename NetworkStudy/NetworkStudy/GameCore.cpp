#include "GameCore.h"
#include "GameObject.h"
#include <windows.h>

void GameCore::Initialize(std::string inName)
{
	mGameObject = std::make_shared<GameObject>(inName);
}

void GameCore::GameLoop()
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		mGameObject->AddCount();
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		mGameObject->SayInfo();
	}
}

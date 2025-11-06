#include "GameObject.h"
#include "MemoryStream.h"

void DrawRectangle(int32 y, int32 x, HDC inHdc)
{
	Rectangle(inHdc, x - 25, y - 25, x + 25, y + 25);
}

Player::Player()
	: mY(0.f), mX(0.f)
{

}

void Player::Write(OutputMemoryStream& inStream) const
{
	inStream.Write(mY);
	inStream.Write(mX);
}

void Player::Update()
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		mX--;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		mX++;
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		mY--;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		mY++;
	}
}

void Player::Render(HDC inHDC)
{
	DrawRectangle(mY, mX, inHDC);
}

void Player::SetPosition(int32 inY, int32 inX)
{
	mY = inY;
	mX = inX;
}


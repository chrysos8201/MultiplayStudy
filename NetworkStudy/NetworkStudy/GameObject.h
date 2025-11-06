#pragma once
#include <iostream>
#include "GameCore.h"
#include "Core.h"

class OutputMemoryStream;
class InputMemoryStream;

class GameObject
{
public:
	CLASS_IDENTIFICATION('GOBJ', GameObject);

	virtual void Update() {}
	virtual void Render(HDC inHDC) {}
private:
};

class Player : public GameObject
{
public:
	Player();

	CLASS_IDENTIFICATION('PLER', Player);
	void Write(OutputMemoryStream& inStream) const;

	virtual void Update() override;
	virtual void Render(HDC inHDC) override;

	void SetPosition(int32 inY, int32 inX);
private:
	int32 mY;
	int32 mX;
};
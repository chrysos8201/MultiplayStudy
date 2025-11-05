#pragma once
#include <iostream>
#include <string>
#include "GameCore.h"

class OutputMemoryStream;
class InputMemoryStream;

class GameObject
{
public:
	CLASS_IDENTIFICATION('GOBJ', GameObject)

private:
};

class Player : public GameObject
{
public:
	CLASS_IDENTIFICATION('PLER', Player);
	void Write(OutputMemoryStream& inStream) const;

private:
	float mY;
	float mX;
};
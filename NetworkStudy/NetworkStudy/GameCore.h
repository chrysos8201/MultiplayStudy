#pragma once
#include <memory>
#include <string>

class GameObject;

class GameCore
{
public:
	void Initialize(std::string inName);
	void GameLoop();

private:
	std::shared_ptr<GameObject> mGameObject;
};


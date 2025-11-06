#pragma once
#include "Core.h"

class GameObject;

class LinkingContext
{
public:
	LinkingContext() : mNextNetworkId(1){}
	uint32 GetNetworkId(const std::shared_ptr<GameObject> inGameObject, bool inShouldCreateifNotFound);

	void AddGameObject(std::shared_ptr<GameObject> inGameObject, uint32 inNetworkId);
	void RemoveGameObject(std::shared_ptr<GameObject> inGameObject);

	std::shared_ptr<GameObject> GetGameObject(uint32 inNetworkId);

private:
	std::unordered_map<uint32, std::shared_ptr<GameObject>> mNetworkIdToGameObjectMap;
	std::unordered_map<std::shared_ptr<GameObject>, uint32> mGameObjectToNetworkIdMap;
	uint32 mNextNetworkId;
};


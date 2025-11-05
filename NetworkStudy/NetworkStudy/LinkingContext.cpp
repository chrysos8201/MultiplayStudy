#include "LinkingContext.h"
#include "GameObject.h"

uint32 LinkingContext::GetNetworkId(const std::shared_ptr<GameObject> inGameObject, bool inShouldCreateifNotFound)
{
	auto it = mGameObjectToNetworkIdMap.find(inGameObject);
	if (it != mGameObjectToNetworkIdMap.end())
	{
		return it->second;
	}
	else if (inShouldCreateifNotFound)
	{
		uint32 newNetworkId = mNextNetworkId++;
		AddGameObject(inGameObject, newNetworkId);
		return newNetworkId;
	}
	else
		return 0;
}

void LinkingContext::AddGameObject(std::shared_ptr<GameObject> inGameObject, uint32 inNetworkId)
{
	mNetworkIdToGameObjectMap[inNetworkId] = inGameObject;
	mGameObjectToNetworkIdMap[inGameObject] = inNetworkId;
}

void LinkingContext::RemoveGameObject(std::shared_ptr<GameObject> inGameObject)
{
	uint32 networkId = mGameObjectToNetworkIdMap[inGameObject];
	mGameObjectToNetworkIdMap.erase(inGameObject);
	mNetworkIdToGameObjectMap.erase(networkId);
}

std::shared_ptr<GameObject> LinkingContext::GetGameObject(uint32 inNetwrokId)
{
	auto it = mNetworkIdToGameObjectMap.find(inNetwrokId);
	if (it != mNetworkIdToGameObjectMap.end())
	{
		return it->second;
	}
	return nullptr;
}


#pragma once
#include "Core.h"
#include "GameObject.h"

using GameObjectCreationFunc = GameObject* (*)();

class ObjectCreationRegistry
{
public:
	static ObjectCreationRegistry& Get()
	{
		static ObjectCreationRegistry sInstance;
		return sInstance;
	}

	template<typename T>
	void RegisterCreationFunction()
	{
		// 중복된 클래스 ID가 없어야 함
		assert(mNameToGameObjectCreationFunctionMap.find(T::kClassId) == mNameToGameObjectCreationFunctionMap.end());

		mNameToGameObjectCreationFunctionMap[T::kClassId] = T::CreateInstance;
	}

	GameObject* CreateGameObject(uint32 inClassId)
	{
		GameObjectCreationFunc creationFunc = mNameToGameObjectCreationFunctionMap[inClassId];
		GameObject* gameObject = creationFunc();
		return gameObject;
	}

private:
	ObjectCreationRegistry() {}
	std::unordered_map<uint32, GameObjectCreationFunc> mNameToGameObjectCreationFunctionMap;
};
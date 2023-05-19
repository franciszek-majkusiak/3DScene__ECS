#pragma once
#include "Coordinator.h"
#include "Scene/GameObject.h"

class GameObject;

class Script
{
public:
	virtual void OnStartUp() {};
	virtual void OnUpdate(float deltaTime) {};
	virtual void OnLateUpdate(float deltaTime) {};

	template<typename T>
	T& GetComponent()
	{
		return Coordinator.GetComponent<T>(owner->entityId);
	}

	template<>
	Transform& GetComponent<Transform>()
	{
		return owner->transform;
	}
	GameObject* owner = nullptr;
};

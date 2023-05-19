#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <list>
#include <memory>
#include "Components/Transform.h"
#include "Coordinator.h"
#include <functional>

class Script;

class GameObject
{
public:
	GameObject(std::string tag, bool transformGui = true);
	GameObject(std::string tag, Transform transform, bool transformGui = true);
	~GameObject();
	Transform transform;
	Transform worldTransform;
	std::string tag;

	void AddChild(GameObject* childObject);

	template<typename T>
	void AddComponent(T component, bool addGui = true)
	{
		Coordinator.AddComponent<T>(entityId, component);
		if (addGui)
		{
			AddGuiElement<T>();
		}
	}

	template<typename T>
	void RemoveComponent()
	{
		Coordinator.RemoveComponent<T>(entityId);
		m_ComponentsUsed.remove(typeid(T).name());
	}

	void AddScript(Script* script);

	void StartUpSelfAndChild();

	void UpdateSelfAndChild(float deltaTime);

	void LateUpdateSelfAndChild(float deltaTime);

	void RenderGui();

	template<typename T>
	void AddGuiElement()
	{
		m_ComponentsUsed.emplace_back(typeid(T).name());
	}

	ecs::Entity entityId;
private:
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;
	std::vector<Script*> scripts;
	std::list<const char*> m_ComponentsUsed;

	void DrawGuiElementTypeName(const char* typeName)
	{
		bool error = false;
		auto& component = GetComponentWithTypeName(entityId, typeName, &error);
		if(!error)
			component.RenderGui();
	}

	Component& GetComponentWithTypeName(ecs::Entity, const char* typeName, bool* error);
};

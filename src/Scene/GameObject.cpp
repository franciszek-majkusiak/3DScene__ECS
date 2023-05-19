#include "GameObject.h"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneGraph.h"
#include "GameObject.h"
#include "Scripts/Script.h"
#include "Components/Components.h"

GameObject::GameObject(std::string tag, bool transformGui)
{
	this->tag = tag;
	entityId = Coordinator.CreateEntity();
	AddComponent<Transform>(Transform(), transformGui);
}


GameObject::GameObject(std::string tag, Transform transform, bool transformGui) : GameObject(tag, transformGui)
{
	this->transform = transform;
	auto& transformComponent = Coordinator.GetComponent<Transform>(entityId);
	transformComponent.Update(transform);
}

GameObject::~GameObject()
{
	for (int i = 0; i < scripts.size(); i++)
	{
		delete scripts[i];
	}

	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

void GameObject::AddChild(GameObject* childObject)
{
	childObject->parent = this;
	children.push_back(childObject);
}


void GameObject::AddScript(Script* script)
{
	script->owner = this;
	scripts.push_back(script);
}

void GameObject::StartUpSelfAndChild()
{
	for (auto& script : scripts)
	{
		script->OnStartUp();
	}
	auto& transformComponent = Coordinator.GetComponent<Transform>(entityId);
	if (parent != nullptr)
	{
		Transform const& parentTransform = parent->worldTransform;
		worldTransform.position = parentTransform.position + (transform.position.x * parentTransform.Right() + transform.position.y * parentTransform.Up() + transform.position.z * -parentTransform.Front());
		worldTransform.rotation = parentTransform.rotation + transform.rotation;
		worldTransform.scale = parentTransform.scale + transform.scale;
		worldTransform.modelMatrix = worldTransform.GetLocalModelMatrix();
		worldTransform.normalMatrix = worldTransform.GetLocalNormalMatrix();
	}
	else
	{
		worldTransform.modelMatrix = transform.GetLocalModelMatrix();
		worldTransform.normalMatrix = transform.GetLocalNormalMatrix();
		worldTransform.position = transform.position;
		worldTransform.rotation = transform.rotation;
		worldTransform.scale = transform.scale;
	}
	transformComponent.Update(worldTransform);
	for (auto& child : children)
	{
		child->StartUpSelfAndChild();
	}
}

void GameObject::UpdateSelfAndChild(float deltaTime)
{
	for (auto& script : scripts)
	{
		script->OnUpdate(deltaTime);
	}
	auto& transformComponent = Coordinator.GetComponent<Transform>(entityId);
	if (parent != nullptr)
	{
		Transform const& parentTransform = parent->worldTransform;
		worldTransform.position = parentTransform.position + (transform.position.x * parentTransform.Right() + transform.position.y * parentTransform.Up() + transform.position.z * -parentTransform.Front());
		worldTransform.rotation = parentTransform.rotation + transform.rotation;
		worldTransform.scale = transform.scale;
		worldTransform.modelMatrix = worldTransform.GetLocalModelMatrix();
		worldTransform.normalMatrix = worldTransform.GetLocalNormalMatrix();
	}
	else
	{
		worldTransform.modelMatrix = transform.GetLocalModelMatrix();
		worldTransform.normalMatrix = transform.GetLocalNormalMatrix();
		worldTransform.position = transform.position;
		worldTransform.rotation = transform.rotation;
		worldTransform.scale = transform.scale;
	}
	transformComponent.Update(worldTransform);
	for (auto&& child : children)
	{
		child->UpdateSelfAndChild(deltaTime);
	}
}

void GameObject::LateUpdateSelfAndChild(float deltaTime)
{
	for (auto& script : scripts)
	{
		script->OnLateUpdate(deltaTime);
	}
	auto& transformComponent = Coordinator.GetComponent<Transform>(entityId);
	if (parent != nullptr)
	{
		Transform const& parentTransform = parent->worldTransform;
		worldTransform.position = parentTransform.position + (transform.position.x * parentTransform.Right() + transform.position.y * parentTransform.Up() + transform.position.z * -parentTransform.Front());
		worldTransform.rotation = parentTransform.rotation + transform.rotation;
		worldTransform.scale = transform.scale;
		worldTransform.modelMatrix = worldTransform.GetLocalModelMatrix();
		worldTransform.normalMatrix = worldTransform.GetLocalNormalMatrix();
	}
	else
	{
		worldTransform.modelMatrix = transform.GetLocalModelMatrix();
		worldTransform.normalMatrix = transform.GetLocalNormalMatrix();
		worldTransform.position = transform.position;
		worldTransform.rotation = transform.rotation;
		worldTransform.scale = transform.scale;
	}
	transformComponent.Update(worldTransform);
	for (auto&& child : children)
	{
		child->LateUpdateSelfAndChild(deltaTime);
	}
}


void GameObject::RenderGui()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (ImGui::TreeNodeEx(tag.c_str(), flags))
	{
		for (auto&& componentTypeName : m_ComponentsUsed)
		{
			DrawGuiElementTypeName(componentTypeName);
		}
		if (children.size() > 0)
		{
			if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entityId, flags, "children"))
			{
				for (auto&& child : children)
				{
					child->RenderGui();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

Component& GameObject::GetComponentWithTypeName(ecs::Entity entity, const char* typeName, bool* error)
{
	if (typeName == typeid(Transform).name())
	{
		return transform;
	}
	else if (typeName == typeid(Camera).name())
	{
		return Coordinator.GetComponent<Camera>(entity);
	}
	else if (typeName == typeid(DirectionalLight).name())
	{
		return Coordinator.GetComponent<DirectionalLight>(entity);
	}
	else if (typeName == typeid(PointLight).name())
	{
		return Coordinator.GetComponent<PointLight>(entity);
	}
	else if (typeName == typeid(SpotLight).name())
	{
		return Coordinator.GetComponent<SpotLight>(entity);
	}
	else if (typeName == typeid(Fog).name())
	{
		return Coordinator.GetComponent<Fog>(entity);
	}
	else if (typeName == typeid(Vibration).name())
	{
		return Coordinator.GetComponent<Vibration>(entity);
	}
	else
		*error = true;
	return transform;

}

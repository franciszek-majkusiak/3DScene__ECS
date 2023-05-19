#include "SceneGraph.h"


void SceneGraph::AddGameObject(GameObject gameObject)
{
	baseLayer.emplace_back(std::make_unique<GameObject>(gameObject));
}

//void SceneGraph::RemoveGameObject(GameObject* gameObject)
//{
//	baseLayer.remove(gameObject);
//}

void SceneGraph::OnStartUp()
{
	for (auto& gameObject : baseLayer)
	{
		gameObject->StartUpSelfAndChild();
	}
}

void SceneGraph::OnUpdate(float deltaTime)
{
	for (auto& gameObject : baseLayer)
	{
		gameObject->UpdateSelfAndChild(deltaTime);
	}
}

void SceneGraph::OnLateUpdate(float deltaTime)
{
	for (auto& gameObject : baseLayer)
	{
		gameObject->LateUpdateSelfAndChild(deltaTime);
	}
}

GameObject& SceneGraph::CreateGameObject(std::string tag, bool transformGui)
{
	AddGameObject(GameObject(tag, transformGui));
	return *SceneGraph::baseLayer.back().get();
}

GameObject& SceneGraph::CreateGameObject(std::string tag, Transform transform, bool transformGui)
{
	AddGameObject(GameObject(tag, transform, transformGui));
	return *SceneGraph::baseLayer.back().get();
}

void SceneGraph::OnGuiRender()
{	
	ImGui::BeginGroup();
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx("Scene graph", flags);
	if (opened)
	{
		for (auto&& child : baseLayer)
		{
			child->RenderGui();
		}
		ImGui::TreePop();
	}
	ImGui::EndGroup();
}
#include "GameObject.h"
#include <memory>
#include <list>


class SceneGraph
{
public:
	void AddGameObject(GameObject gameObject);

	//static void RemoveGameObject(GameObject gameObject);

	void OnStartUp();

	void OnUpdate(float deltaTime);

	void OnLateUpdate(float deltaTime);

	void OnGuiRender();
	
	std::list<std::unique_ptr<GameObject>> baseLayer;

	GameObject& CreateGameObject(std::string tag, bool transformGui = true);

	GameObject& CreateGameObject(std::string tag, Transform transform, bool transformGui = true);
};

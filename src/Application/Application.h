#pragma once
#include <memory>
#include "Scene/Scene.h"
#include "Window/Window.h"

class Application
{
public:
	Application();
	~Application();
	void Run();

private:
	Scene m_Scene;
	Window* m_Window;
	bool m_IsRunning = true;
	float m_deltaTime;
	float m_lastFrameTime;
};

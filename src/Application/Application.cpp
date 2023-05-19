#include "Application.h"
#include <iostream>
#include "Input/Input.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


Application::Application()
{
	std::cout << "Starting application" << std::endl;
	m_Window = new Window("App");
	m_deltaTime = 0;


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetWindowPtr(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
	ImGui::StyleColorsDark();

	m_Scene.Init(); 
}
Application::~Application()
{
	m_Scene.End();
	delete m_Window;
	std::cout << "Closing application" << std::endl;
}
void Application::Run()
{
	m_Scene.StartUp();

	m_lastFrameTime = static_cast<float>(glfwGetTime());


	float frameCount = 0;
	float firstFrameTime = m_lastFrameTime;
	float FPS = 0.0f;
	while (m_IsRunning)
	{
		// Time logic
		float currentTime = static_cast<float>(glfwGetTime());
		m_deltaTime = currentTime - m_lastFrameTime;
		m_lastFrameTime = currentTime;

		frameCount++;

		if (frameCount == 10)
		{
			frameCount = 0.0f;
			FPS = 10.0f / (currentTime - firstFrameTime);
			firstFrameTime = currentTime;
		}

		if(Input::IsKeyDown(GLFW_KEY_ESCAPE))
			m_IsRunning = false;



		m_Scene.Update(m_deltaTime);


		// ImGui
			// --------
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Settings", 0, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Value("FPS", FPS);
		ImGui::Value("Frame Time", m_deltaTime);
		m_Scene.RenderGui();
		ImGui::End();

		//Render ImGui
		// ------------
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		m_IsRunning =  m_Window->Update();
	}

}
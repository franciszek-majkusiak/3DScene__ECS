#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>




class Window
{
public:
	Window(std::string title, int width = 1600, int height = 900);
	Window() {}
	~Window();

	bool Update();

	inline unsigned int GetWidth() { return m_Width; }
	inline unsigned int GetHeight() { return m_Height; }
	inline GLFWwindow* GetWindowPtr() { return m_Window; }



private:
	GLFWwindow* m_Window;

	std::string m_Title;
	int m_Width;
	int m_Height;
};
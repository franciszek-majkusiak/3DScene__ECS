#pragma once
#include <unordered_set>
#include <GLFW/glfw3.h>


class Input
{
public:
	static bool IsKeyDown(int keyCode);
	static std::pair<float, float> GetMouseMove();
	static float GetScrollOffset();
	static bool IsLPMPressed();
	static bool IsRPMPressed();
	static int GetWindowWidth();
	static int GetWindowHeight();
	static float GetAspectRatio();
	static float KeyboardHorizontalAxis();
	static float KeyboardVerticalAxis();



	// Modify Input
	static void KeyPressed(int keyCode);
	static void KeyReleased(int keyCode);
	static void MouseMoved(float xOffset, float yOffset);
	static void Scrolled(float offset);
	static void LPMPressed();
	static void LPMReleased();
	static void RPMPressed();
	static void RPMReleased();
	static void Reset();
	static void Resize(int width, int height);

	static std::pair<float, float> lastMousePosition;
	static bool isFirstMove;

private:

	static std::unordered_set<int> m_KeysPressed;
	static float m_mouseXOffset;
	static float m_mouseYOffset;
	static float m_mouseScrollOffset;
	static bool m_LPM;
	static bool m_RPM;
	static int m_WindowWidth;
	static int m_WindowHeight;
};
#include "Input.h"

std::unordered_set<int> Input::m_KeysPressed;
float Input::m_mouseXOffset = 0.0f;
float Input::m_mouseYOffset = 0.0f;
float Input::m_mouseScrollOffset = 0.0f;
bool Input::m_LPM = false;
bool Input::m_RPM = false;
std::pair<float, float> Input::lastMousePosition = std::pair<float, float>(0.0f, 0.0f);
bool Input::isFirstMove = true;
int Input::m_WindowWidth = 0;
int Input::m_WindowHeight = 0;


void Input::KeyPressed(int keyCode)
{
	Input::m_KeysPressed.insert(keyCode);
}
void Input::KeyReleased(int keyCode)
{
	Input::m_KeysPressed.erase(keyCode);
}
void Input::MouseMoved(float xOffset, float yOffset)
{
	Input::m_mouseXOffset = xOffset;
	Input::m_mouseYOffset = yOffset;
}
void Input::Scrolled(float offset)
{
	Input::m_mouseScrollOffset = offset;
}
void Input::LPMPressed()
{
	Input::m_LPM = true;
}
void Input::LPMReleased()
{
	Input::m_LPM = false;
}

void Input::RPMPressed()
{
	Input::m_RPM = true;
}
void Input::RPMReleased()
{
	Input::m_RPM = false;
}

void Input::Resize(int width, int height)
{
	Input::m_WindowWidth = width;
	Input::m_WindowHeight = height;
}

void Input::Reset()
{
	//Input::m_LPM = false;
	//Input::m_RPM = false;
	Input::m_mouseXOffset = 0.0f;
	Input::m_mouseYOffset = 0.0f;
	Input::m_mouseScrollOffset = 0.0f;

}

bool Input::IsKeyDown(int keyCode)
{
	return Input::m_KeysPressed.contains(keyCode);
}
std::pair<float, float> Input::GetMouseMove()
{
	//glfwPollEvents();
	return std::pair<float, float>(Input::m_mouseXOffset, Input::m_mouseYOffset);
}
float Input::GetScrollOffset()
{
	return Input::m_mouseScrollOffset;
}
bool Input::IsLPMPressed()
{
	return Input::m_LPM;
}
bool Input::IsRPMPressed()
{
	return Input::m_RPM;
}
int Input::GetWindowWidth()
{
	return Input::m_WindowWidth;
}
int Input::GetWindowHeight()
{
	return Input::m_WindowHeight;
}

float Input::GetAspectRatio()
{
	return (float)Input::m_WindowWidth / (float)Input::m_WindowHeight;
}

float Input::KeyboardHorizontalAxis()
{
	float value = 0.0f;
	if (Input::IsKeyDown(GLFW_KEY_D))
		value += 1.0f;
	if (Input::IsKeyDown(GLFW_KEY_A))
		value -= 1.0f;
	if (value > 1.0) value = 1.0f;
	if (value < -1.0) value = -1.0f;
	return value;
}
float Input::KeyboardVerticalAxis()
{
	float value = 0.0f;
	if (Input::IsKeyDown(GLFW_KEY_W))
		value += 1.0f;
	if (Input::IsKeyDown(GLFW_KEY_S))
		value -= 1.0f;
	if (value > 1.0) value = 1.0f;
	if (value < -1.0) value = -1.0f;
	return value;
}

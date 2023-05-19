#pragma once
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
struct Component
{
	virtual void RenderGui() {};
};

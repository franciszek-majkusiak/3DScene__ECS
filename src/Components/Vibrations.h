#pragma once
#include <glm/glm.hpp>
#include "Component.h"

struct Vibration : Component
{
	float amplitude = 1.0f;
	float speed = 10.0f;
	bool isEnabled = false;

	void RenderGui() override 
	{
		ImGui::BeginGroup();
		ImGui::Text("Vibration");
		ImGui::Checkbox("Is enabled", &isEnabled);
		ImGui::DragFloat("Amplitude", &amplitude, 0.2f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Speed", &speed, 0.2f, 0.0f, 0.0f, "%.2f");
	}

};

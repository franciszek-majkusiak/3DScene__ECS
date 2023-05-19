#pragma once
#include <glm/glm.hpp>
#include "Component.h"

struct Fog : Component
{
	glm::vec3 color = glm::vec3(1);
	float linearStart = 50.0f;
	float linearEnd = 100.0f;
	float density = 0.04f;
	int equation = 0;
	bool isEnabled = false;

	void RenderGui() override
	{
		ImGui::BeginGroup();
		ImGui::Text("Fog");
		ImGui::Checkbox("Is Enabled", &isEnabled);
		const char* currentItem = types[equation];
		if (ImGui::BeginCombo("Fog types", currentItem))
		{
			for (int i = 0; i < 3; i++)
			{
				bool is_selected = (currentItem == types[i]);
				if (ImGui::Selectable(types[i], is_selected))
				{
					currentItem = types[i];
					equation = i;
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::DragFloat3("Color", glm::value_ptr<float>(color), 0.1f, 0.0f, 1.0f, "%.2f");
		ImGui::DragFloat("Density", &density, 0.0001f, 0.0f, 0.0f, "%.4f");
		ImGui::DragFloat("Linear start", &linearStart, 0.2f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Linear end", &linearEnd, 0.2f, 0.0f, 0.0f, "%.2f");
		ImGui::EndGroup();
	}
private:
	const char* types[3] = { "Linear" , "Expotential" , "Expotential quadratic" };
};

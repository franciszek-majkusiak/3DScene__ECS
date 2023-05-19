#pragma once
#include <glm/glm.hpp>
#include "Component.h"


struct PointLight : Component
{
	glm::vec3 ambient = glm::vec3(0.2f);
	glm::vec3 diffuse = glm::vec3(1);
	glm::vec3 specular = glm::vec3(1);

	float constant = 1.0f;
	float linear = 0.01f;
	float quadratic = 0.005f;

	float radius = 0.0f;


	void UpdateRadius(float minBrightness = 0.005f)
	{
		float maxIntensity = std::fmax(std::fmax(std::fmax(diffuse.x, diffuse.y), diffuse.z), std::fmax(std::fmax(specular.x, specular.y), specular.z));
		radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - 1/minBrightness * maxIntensity))) / (2 * quadratic);
	}
};


struct DirectionalLight : Component
{
	glm::vec3 direction = glm::vec3(0, -1, 0);

	glm::vec3 ambient = glm::vec3(0.2f);
	glm::vec3 diffuse = glm::vec3(1);
	glm::vec3 specular = glm::vec3(1);
};


struct SpotLight : Component 
{
	glm::vec3 ambient = glm::vec3(0.2f);
	glm::vec3 diffuse = glm::vec3(1);
	glm::vec3 specular = glm::vec3(1);

	float constant = 1.0f;
	float linear = 0.001f;
	float quadratic = 0.0005f;

	float cutOff = 12.5f;
	float outerCutOff = 17.5f;


	void RenderGui() override
	{
		ImGui::Text("Spot Light");
		ImGui::Text("Colors");
		ImGui::DragFloat3("Ambient", glm::value_ptr<float>(ambient), 0.1f, 0.0f, 1.0f, "%.2f");
		ImGui::DragFloat3("Diffuse", glm::value_ptr<float>(diffuse), 0.1f, 0.0f, 1.0f, "%.2f");
		ImGui::DragFloat3("Specular", glm::value_ptr<float>(specular), 0.1f, 0.0f, 1.0f, "%.2f");
		ImGui::Text("Attenuation");
		ImGui::DragFloat("Constant", &constant, 0.0001f, 0.0f, 0.0f, "%.4f");
		ImGui::DragFloat("Linear", &linear, 0.001f, 0.0f, 1.0f, "%.3f");
		ImGui::DragFloat("Quadratic", &quadratic, 0.001f, 0.0f, 1.0f, "%.3f");
		ImGui::Text("Shape");
		ImGui::DragFloat("Cut Off", &cutOff, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Outer cut off", &outerCutOff, 0.1f, 0.0f, 0.0f, "%.2f");
	}
};
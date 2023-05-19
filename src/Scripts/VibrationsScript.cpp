#include "VibrationsScript.h"
#include "Components/Vibrations.h"
#include "Components/Transform.h"
#include "Coordinator.h"
#include <glm/gtc/constants.hpp>

void VibrationsScript::OnStartUp()
{
}
void VibrationsScript::OnUpdate(float deltaTime)
{
	auto& transform = GetComponent<Transform>();
	auto& vibration = GetComponent<Vibration>();
	if (!vibration.isEnabled) return;


	float lastPos = glm::sin(vibration.speed * glm::pi<float>() * currentTime);
	float curPos = glm::sin(vibration.speed * glm::pi<float>() * currentTime + deltaTime);
	float toMove = curPos - lastPos;
	currentTime += deltaTime;

	transform.position.y = glm::sin(currentTime * vibration.speed * glm::pi<float>()) * vibration.amplitude + vibration.amplitude;
}
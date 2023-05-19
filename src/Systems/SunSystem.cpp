#include "SunSystem.h"
#include "Coordinator.h"
#include "Components/Light.h"
#include "Components/Transform.h"


void SunSystem::Init()
{
	signature.set(Coordinator.GetComponentType<DirectionalLight>());
	Coordinator.SetSystemSignature<SunSystem>(signature);

}

void SunSystem::OnStartUp(float rotationSpeed)
{
	m_TimeForFullRotation = rotationSpeed;
}

void SunSystem::OnUpdate(float deltaTime)
{
	for (auto const& entity : Entities)
	{
		auto& dirLight = Coordinator.GetComponent<DirectionalLight>(entity);


		float angleVelocity = (2 * glm::pi<float>() * deltaTime) / m_TimeForFullRotation;

		dirLight.direction = glm::rotateZ(dirLight.direction, angleVelocity);

		float angle = glm::angle(dirLight.direction, glm::vec3(0, -1, 0));

		float k = glm::cos(angle);
		if (k < 0.0f)
			dirLight.diffuse = glm::vec3(0);
		else
			dirLight.diffuse = glm::vec3(std::sqrtf(1-10*angle*angle/(8*glm::pi<float>())), k, k);
		dirLight.specular = glm::vec3(std::fmaxf(glm::cos(angle), 0.0f));
		dirLight.ambient = glm::vec3(std::fmaxf(glm::cos(angle), 0.0f));

	}
}
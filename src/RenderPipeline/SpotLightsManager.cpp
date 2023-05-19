#include "SpotLightsManager.h"
#include "Coordinator.h"
#include "Components/Transform.h"
#include "Components/Light.h"


void SpotLightsManager::Init()
{
	signature.set(Coordinator.GetComponentType<SpotLight>());
	signature.set(Coordinator.GetComponentType<Transform>());
	Coordinator.SetSystemSignature<SpotLightsManager>(signature);
}
void SpotLightsManager::OnUpdate(std::vector<Shader>& shaders)
{
	int i = 0;
	for (auto const& entity : Entities)
	{
		auto& spotLight = Coordinator.GetComponent<SpotLight>(entity);
		auto const& transform = Coordinator.GetComponent<Transform>(entity);

		std::string uniformFirstPart = "spotLights[" + std::to_string(i) + "].";
		for (Shader& shader : shaders)
		{
			shader.Activate();
			shader.setVec3(uniformFirstPart + "position", transform.position);
			shader.setVec3(uniformFirstPart + "direction", transform.Front());
			shader.setVec3(uniformFirstPart + "ambient", spotLight.ambient);
			shader.setVec3(uniformFirstPart + "diffuse", spotLight.diffuse);
			shader.setVec3(uniformFirstPart + "specular", spotLight.specular);
			shader.setFloat(uniformFirstPart + "constant", spotLight.constant);
			shader.setFloat(uniformFirstPart + "linear", spotLight.linear);
			shader.setFloat(uniformFirstPart + "quadratic", spotLight.quadratic);
			shader.setFloat(uniformFirstPart + "cutOff", glm::cos(glm::radians(spotLight.cutOff)));
			shader.setFloat(uniformFirstPart + "outerCutOff", glm::cos(glm::radians(spotLight.outerCutOff)));
		}
		i++;
	}
	for (Shader& shader : shaders)
	{
		shader.Activate();
		shader.setInt("numOfSpotLights", i);
	}
}
#include "DirectionalLightsManager.h"
#include "RenderContext.h"
#include "Coordinator.h"
#include "Components/Transform.h"
#include "Components/Light.h"


void DirectionalLightsManager::Init()
{
	signature.set(Coordinator.GetComponentType<DirectionalLight>());
	Coordinator.SetSystemSignature<DirectionalLightsManager>(signature);
}
void DirectionalLightsManager::OnUpdate(std::vector<Shader>& shaders)
{
	for (auto const& entity : Entities)
	{
		auto& dirLight = Coordinator.GetComponent<DirectionalLight>(entity);


		std::string uniformFirstPart = "dirLight.";

		for (Shader& shader : shaders)
		{
			shader.Activate();
			shader.setVec3(uniformFirstPart + "direction", dirLight.direction);
			shader.setVec3(uniformFirstPart + "ambient", dirLight.ambient);
			shader.setVec3(uniformFirstPart + "diffuse", dirLight.diffuse);
			shader.setVec3(uniformFirstPart + "specular", dirLight.specular);
		}
	}
}
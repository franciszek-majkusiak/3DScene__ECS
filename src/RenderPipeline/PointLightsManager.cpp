#include "PointLightsManager.h"
#include "RenderContext.h"
#include "Coordinator.h"
#include "Components/Transform.h"
#include "Components/Light.h"


void PointLightsManager::Init()
{
	signature.set(Coordinator.GetComponentType<PointLight>());
	signature.set(Coordinator.GetComponentType<Transform>());
	Coordinator.SetSystemSignature<PointLightsManager>(signature);
}
void PointLightsManager::OnUpdate(std::vector<Shader>& shaders)
{
	int i = 0;
	for (auto const& entity : Entities)
	{
		auto& pointLight = Coordinator.GetComponent<PointLight>(entity);
		auto const& transform = Coordinator.GetComponent<Transform>(entity);

		std::string uniformFirstPart = "pointLights[" + std::to_string(i) + "].";
		for (Shader& shader : shaders)
		{
			shader.Activate();
			shader.setVec3(uniformFirstPart + "position", transform.position);
			shader.setVec3(uniformFirstPart + "ambient", pointLight.ambient);
			shader.setVec3(uniformFirstPart + "diffuse", pointLight.diffuse);
			shader.setVec3(uniformFirstPart + "specular", pointLight.specular);
			shader.setFloat(uniformFirstPart + "constant", pointLight.constant);
			shader.setFloat(uniformFirstPart + "linear", pointLight.linear);
			shader.setFloat(uniformFirstPart + "quadratic", pointLight.quadratic);
			shader.setFloat(uniformFirstPart + "radius", pointLight.radius);
		}
		i++;
	}
	for (Shader& shader : shaders)
	{
		shader.Activate();
		shader.setInt("numOfPointLights", i);
	}
}
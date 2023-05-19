#include "FogSystem.h"
#include "Coordinator.h"
#include "Components/Fog.h"


void FogSystem::Init()
{
	signature.set(Coordinator.GetComponentType<Fog>());
	Coordinator.SetSystemSignature<FogSystem>(signature);
}


void FogSystem::OnUpdate(std::vector<Shader>& shaders)
{
	for (auto const& entity : Entities)
	{
		auto const& fog = Coordinator.GetComponent<Fog>(entity);

		for (Shader& shader : shaders)
		{
			shader.Activate();
			shader.setVec3("fog.color", fog.color);
			shader.setFloat("fog.linearStart", fog.linearStart);
			shader.setFloat("fog.linearEnd", fog.linearEnd);
			shader.setFloat("fog.density", fog.density);
			shader.setInt("fog.equation", fog.equation);
			shader.setBool("fog.isEnabled", fog.isEnabled);
		}

		break;
	}
}
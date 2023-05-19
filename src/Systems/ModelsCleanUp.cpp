#include "ModelsCleanUp.h"
#include "Components/Mesh.h"
#include "Coordinator.h"

void ModelsCleanUp::Init()
{
	signature.set(Coordinator.GetComponentType<Model>());
	Coordinator.SetSystemSignature<ModelsCleanUp>(signature);
}
void ModelsCleanUp::OnEnd()
{
	for (auto const& entity : Entities)
	{
		auto& model = Coordinator.GetComponent<Model>(entity);
		if (model.isPrefab) continue;
		for (Mesh& mesh : model.meshes)
		{
			//delete[] mesh.indices;
			//delete[] mesh.vertices;
		}
	}
}
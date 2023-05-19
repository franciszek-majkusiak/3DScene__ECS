#include "InstancedModelsCleanUp.h"
#include "Components/InstancedModel.h"
#include "Coordinator.h"

void InstancedModelsCleanUp::Init()
{
	signature.set(Coordinator.GetComponentType<InstancedModel>());
	Coordinator.SetSystemSignature<InstancedModelsCleanUp>(signature);
}
void InstancedModelsCleanUp::OnEnd()
{
	for (auto const& entity : Entities)
	{
		auto& model = Coordinator.GetComponent<InstancedModel>(entity);
		//delete[] model.modelMatrices;
	}
}
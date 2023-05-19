#pragma once
#include "ECS/System.h"

class InstancedModelsCleanUp : public ecs::System
{
public:
	void Init();
	void OnEnd();
};

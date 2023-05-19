#pragma once
#include "ECS/System.h"

class ModelsCleanUp : public ecs::System
{
public:
	void Init();
	void OnEnd();
};

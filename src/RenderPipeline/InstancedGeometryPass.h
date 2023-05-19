#pragma once
#include "ECS/System.h"
#include "Shaders/Shader.h"
#include "Components/Transform.h"


class InstancedGeometryPass : public ecs::System
{
public:
	void Init();
	void OnUpdate(Shader shader);
};

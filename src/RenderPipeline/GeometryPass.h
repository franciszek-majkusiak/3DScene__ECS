#pragma once
#include "ECS/System.h"
#include "Shaders/Shader.h"
#include "Components/Transform.h"


class GeometryPass : public ecs::System
{
public:
	void Init();
	void OnUpdate(Shader shader);
};

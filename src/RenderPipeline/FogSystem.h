#pragma once
#include "ECS/System.h"
#include "Shaders/Shader.h"
#include <vector>

class FogSystem : public ecs::System
{
public:
	void Init();
	void OnUpdate(std::vector<Shader>& shaders);
};

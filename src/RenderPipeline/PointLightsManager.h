#pragma once
#include "ECS/System.h"
#include "Shaders/Shader.h"
#include <vector>

class PointLightsManager : public ecs::System
{
public:
	void Init();
	void OnUpdate(std::vector<Shader>& shaders);
};

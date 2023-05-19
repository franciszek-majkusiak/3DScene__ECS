#pragma once
#include "ECS/System.h"
#include "Shaders/Shader.h"
#include <vector>

class CameraManager : public ecs::System
{
public:
	void Init();
	glm::vec3 OnUpdate(std::vector<Shader>& shaders);
};

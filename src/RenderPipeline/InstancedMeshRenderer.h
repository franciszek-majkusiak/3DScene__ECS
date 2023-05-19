#pragma once
#include "ECS/System.h"
#include "Shaders/Shader.h"


class InstancedMeshRenderer : public ecs::System
{
public:
	void Init();
	void OnUpdate(Shader shader);

};

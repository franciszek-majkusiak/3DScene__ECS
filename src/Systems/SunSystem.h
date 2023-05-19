#pragma once
#include "ECS/System.h"
#define GLM_SWIZZLE
#include <glm/glm.hpp>

class SunSystem : public ecs::System
{
public:
	void Init();
	void OnStartUp(float timeForFullRotation);
	void OnUpdate(float deltatime);

private:
	float m_TimeForFullRotation;
};
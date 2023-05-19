#pragma once
#include <glm/glm.hpp>
#include "Component.h"

struct Camera : Component
{
	float FOV = 45.0f;
	float near = 0.1f;
	float far = 500.0f;
};


struct MainCamera
{

};

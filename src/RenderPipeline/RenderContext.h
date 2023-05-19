#pragma once
#include "Components/Light.h"
#include <vector>

class RenderContext
{
public:
	// Lights
	static std::vector<PointLight> pointLights;

	// camera
	static glm::mat4 viewMatrix;
	static glm::mat4 projectionMatrix;
	static glm::vec3 cameraPosition;



	static void Reset();
};

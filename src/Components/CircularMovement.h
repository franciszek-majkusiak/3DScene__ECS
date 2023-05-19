#pragma once

#include <glm/glm.hpp>


struct CircularMovement
{
	float radius = 1.0f;
	glm::vec3 center = glm::vec3(0);
	glm::vec3 axis = glm::vec3(0,1,0);

	float rotationSpeed = 1.0f;
};

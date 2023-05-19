#include "RenderContext.h"


std::vector<PointLight> RenderContext::pointLights = std::vector<PointLight>(10);

glm::mat4 RenderContext::viewMatrix = glm::mat4(1);
glm::mat4 RenderContext::projectionMatrix = glm::mat4(1);
glm::vec3 RenderContext::cameraPosition = glm::vec3(1);



static void Reset()
{
	RenderContext::pointLights.clear();
	RenderContext::viewMatrix = glm::mat4(1);
	RenderContext::projectionMatrix = glm::mat4(1);
	RenderContext::cameraPosition = glm::vec3(1);
}
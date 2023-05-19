#include "CameraManager.h"
#include "RenderContext.h"
#include "Coordinator.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Input/Input.h"


void CameraManager::Init()
{
	signature.set(Coordinator.GetComponentType<Camera>());
	signature.set(Coordinator.GetComponentType<MainCamera>());
	signature.set(Coordinator.GetComponentType<Transform>());
	Coordinator.SetSystemSignature<CameraManager>(signature);
}
glm::vec3 CameraManager::OnUpdate(std::vector<Shader>& shaders)
{
	for (auto const& entity : Entities)
	{
		auto const& camera = Coordinator.GetComponent<Camera>(entity);
		auto const& transform = Coordinator.GetComponent<Transform>(entity);

		glm::mat4 projectionMatrix = glm::perspective(camera.FOV, Input::GetAspectRatio(), camera.near, camera.far);
		glm::mat4 viewMatrix = glm::lookAt(transform.position, transform.position + transform.Front(), transform.Up());


		for (Shader& shader : shaders)
		{
			shader.Activate();
			shader.setMat4("view", viewMatrix);
			shader.setMat4("projection", projectionMatrix);
			shader.setVec3("viewPos", transform.position);
			// Only for skybox
			shader.setVec3("position", transform.position);
		}
		return transform.position;
	}
}
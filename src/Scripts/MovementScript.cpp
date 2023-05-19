#include "Scripts/MovementScript.h"
#include "Input/Input.h"
#include <glm/glm.hpp>

void MovementScript::OnStartUp()
{
}
void MovementScript::OnUpdate(float deltaTime)
{
	auto& transform = GetComponent<Transform>();
	speed += Input::GetScrollOffset();


	float xMove = Input::KeyboardHorizontalAxis();
	float zMove = Input::KeyboardVerticalAxis();
	float yMove = 0.0f;
	yMove += Input::IsKeyDown(GLFW_KEY_SPACE) ? 1.0f : 0.0f;
	yMove += Input::IsKeyDown(GLFW_KEY_LEFT_CONTROL) ? -1.0f : 0.0f;

	if (xMove != 0 || zMove != 0 || yMove != 0)
	{
		glm::vec3 velocity = deltaTime * speed * (transform.Front() * zMove + transform.Right() * xMove + transform.Up() * yMove);
		transform.Translate(velocity);
	}


	auto mouseOffset = Input::GetMouseMove();
	float yRot = mouseOffset.second;
	float xRot = mouseOffset.first;
	camera->transform.Rotate(mouseSensitivity * deltaTime * yRot, camera->transform.Right());
	transform.Rotate(mouseSensitivity * deltaTime * -xRot, glm::vec3(0, 1, 0));
}
#include "Scripts/EditorCameraMovementScript.h"
#include "Input/Input.h"

void EditorCameraMovement::OnStartUp()
{

}

void EditorCameraMovement::OnLateUpdate(float deltaTime)
{
	auto& transform = GetComponent<Transform>();

	float front = Input::GetScrollOffset();

	float horizontal = 0.0f, vertical = 0.0f;
	if (Input::IsLPMPressed())
	{
		horizontal= Input::GetMouseMove().first;
		vertical = Input::GetMouseMove().second;
	}


	float horizontalRot = 0.0f, verticalRot = 0.0f;
	if (Input::IsRPMPressed())
	{
		horizontalRot = Input::GetMouseMove().first;
		verticalRot = Input::GetMouseMove().second;
	}

	transform.Translate(speed * deltaTime * (transform.Front() * 50.0f * front + transform.Right() * -horizontal + transform.Up() * -vertical));
	transform.Rotate(-horizontalRot * speed * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
	transform.Rotate(verticalRot * speed*  deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
}
#include "CarMovementScript.h"
#include "Input/Input.h"
#include <glm/glm.hpp>

void CarMovementController::OnStartUp()
{
}
void CarMovementController::OnUpdate(float deltaTime)
{
	auto& transform = GetComponent<Transform>();
	//speed += Input::GetScrollOffset();


	float rotation = Input::KeyboardHorizontalAxis();
	float zMove = Input::KeyboardVerticalAxis();

	if (zMove != 0)
	{
		glm::vec3 velocity = deltaTime * speed * (transform.Front() * zMove);
		transform.Translate(velocity);
	}


	/*auto mouseOffset = Input::GetMouseMove();
	float yRot = mouseOffset.second;
	float xRot = mouseOffset.first;*/
	transform.Rotate(steeringPower * deltaTime * -rotation, glm::vec3(0, 1, 0));
}
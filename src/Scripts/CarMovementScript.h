#include "Script.h"
#include "Components/Transform.h"

class CarMovementController : public Script
{
public:
	void OnStartUp() override;
	void OnUpdate(float deltaTime) override;

	float speed = 20.0f;
	float steeringPower = 100.0f;

	GameObject* camera;

private:

};

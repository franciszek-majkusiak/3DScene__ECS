#include "Script.h"
#include "Components/Transform.h"

class MovementScript : public Script
{
public:
	void OnStartUp() override;
	void OnUpdate(float deltaTime) override;

	float speed = 10.0f;
	float mouseSensitivity = 10.0f;

	GameObject* camera;

private:

};

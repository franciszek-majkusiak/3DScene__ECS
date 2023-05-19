#include "Script.h"
#include "Components/Transform.h"

class EditorCameraMovement : public Script
{
public:
	void OnStartUp() override;
	void OnLateUpdate(float deltaTime) override;


	float speed = 10.0f;
private:

};

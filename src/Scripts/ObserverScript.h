#include "Script.h"
#include "Components/Transform.h"

class ObserverScript : public Script
{
public:
	void OnStartUp() override;
	void OnLateUpdate(float deltaTime) override;

	GameObject* target;

private:

};

#pragma once
#include "Script.h"

class VibrationsScript : public Script
{
public:
	void OnStartUp() override;
	void OnUpdate(float deltaTime) override;

private:
	float currentTime = 0.0f;
};
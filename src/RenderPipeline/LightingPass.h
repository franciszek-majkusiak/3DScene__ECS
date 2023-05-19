#pragma once
#include "ECS/System.h"
#include "Shaders/Shader.h"
#include <vector>


class LightingPass : public ecs::System
{
public:
	void Init();
	void OnUpdate(Shader shader);

private:
	std::vector<float> m_rectVertices;
	GLuint m_VAO;
	GLuint m_VBO;
};

#pragma once
#include "Shaders/Shader.h"
#include <vector>

class SkyBox
{
public:
	SkyBox() {}
	SkyBox(std::vector<std::string>& faces);
	~SkyBox();
	void Load(std::vector<std::string>& faces);
	Shader shader;

	void Render();

private:
	unsigned int cubeMapId;
	float* skyboxVertices;
	GLuint VAO;
	GLuint VBO;
};

#pragma once
#include <glad/glad.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void Init(unsigned int widht, unsigned int height);

	void BindForWriting();
	void BindForReading(GLenum textureUnit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};
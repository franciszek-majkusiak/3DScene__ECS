#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"


struct Transform : Component
{
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	glm::mat4 modelMatrix = glm::mat4(1);
	glm::mat4 normalMatrix = glm::mat4(1);

	Transform(glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(0));

	void Translate(glm::vec3 translationVector);

	void Rotate(float angle, glm::vec3 axis);

	void RotateRadians(float angle, glm::vec3 axis);

	void Rotate(glm::vec3 rotation);

	void Scale(glm::vec3 scaleVector);

	glm::mat4 GetLocalModelMatrix();
	glm::mat4 GetLocalNormalMatrix();

	void Update(Transform& transform);
	

	glm::vec3 Front() const;
	glm::vec3 Up() const;
	glm::vec3 Right() const;

	void RenderGui() override;
};
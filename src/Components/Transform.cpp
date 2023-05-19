#include "Transform.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	Translate(position);
	Rotate(rotation);
	Scale(scale);
}

void Transform::Translate(glm::vec3 translationVector)
{
	position += translationVector;
	modelMatrix = GetLocalModelMatrix();
	normalMatrix = GetLocalNormalMatrix();
}

void Transform::Rotate(float angle, glm::vec3 axis)
{
	rotation += angle * axis;
	rotation = glm::vec3(fmodf(rotation.x,360.0f), fmodf(rotation.y, 360.0f), fmodf(rotation.z, 360.0f));
	modelMatrix = GetLocalModelMatrix();
	normalMatrix = GetLocalNormalMatrix();
}

void Transform::RotateRadians(float angle, glm::vec3 axis)
{
	angle = glm::degrees(angle);
	Rotate(angle, axis);
}

void Transform::Rotate(glm::vec3 rot)
{
	rotation += rot;
	rotation = glm::vec3(fmodf(rotation.x, 360.0f), fmodf(rotation.y, 360.0f), fmodf(rotation.z, 360.0f));
	modelMatrix = GetLocalModelMatrix();
	normalMatrix = GetLocalNormalMatrix();
}

void Transform::Scale(glm::vec3 scaleVector)
{
	scale += scaleVector;
	modelMatrix = GetLocalModelMatrix();
	normalMatrix = GetLocalNormalMatrix();
}

glm::mat4 RotationMatrix(glm::vec3 rotation)
{
	//return glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	return glm::toMat4(glm::quat(glm::radians(rotation)));
	//return glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
}

glm::mat4 Transform::GetLocalModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), position) * RotationMatrix(rotation) * glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 Transform::GetLocalNormalMatrix()
{
	return glm::transpose(glm::inverse(GetLocalModelMatrix()));
}

void Transform::Update(Transform& transform)
{
	position = transform.position;
	rotation = transform.rotation;
	scale = transform.scale;
	modelMatrix = transform.modelMatrix;
	normalMatrix = transform.normalMatrix;
}


glm::vec3 Transform::Front() const
{
	return RotationMatrix(rotation) * glm::vec4(glm::vec3(0.0f, 0.0f, -1.0f), 1.0f);
}
glm::vec3 Transform::Up() const
{
	return RotationMatrix(rotation) * glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
}

glm::vec3 Transform::Right() const
{
	return RotationMatrix(rotation) * glm::vec4(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
}

void Transform::RenderGui()
{
	ImGui::BeginGroup();
	ImGui::Text("Transform");
	ImGui::DragFloat3("Position", glm::value_ptr<float>(position), 0.2f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat3("Rotation", glm::value_ptr<float>(rotation), 0.2f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat3("Scale", glm::value_ptr<float>(scale), 0.2f, 0.0f, 0.0f, "%.2f");
	ImGui::EndGroup();
}
#include "ObserverScript.h"
#include "Input/Input.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

void ObserverScript::OnStartUp()
{
	auto& transform = GetComponent<Transform>();

	glm::vec3 frontVec = transform.Front();
	glm::vec3 desiredFront = glm::normalize(target->worldTransform.position - transform.position);

	glm::vec3 axis = glm::normalize(glm::cross(frontVec, desiredFront));
	float angle = glm::orientedAngle(frontVec, desiredFront, axis);
	auto frontBefore = transform.Front();
	transform.RotateRadians(angle, axis);
	auto frontAfter = transform.Front();

	axis = glm::normalize(glm::cross(transform.Front(), glm::vec3(0, 1, 0)));
	glm::mat4 rotMat = glm::rotate(glm::pi<float>() / 2.0f, axis);
	glm::vec3 desiredUp = rotMat * glm::vec4(transform.Front(), 1.0f);

	float angle2 = glm::orientedAngle(transform.Up(), desiredUp, transform.Front());
	auto frontBefore2 = transform.Front();
	transform.RotateRadians(angle2, transform.Front());
	auto frontAfter2 = transform.Front();
}
void ObserverScript::OnLateUpdate(float deltaTime)
{
	auto& transform = GetComponent<Transform>();

	glm::vec3 frontVec = transform.Front();
	glm::vec3 desiredFront = glm::normalize(target->worldTransform.position - transform.position);

	glm::vec3 axis = glm::normalize(glm::cross(frontVec, desiredFront));
	float angle = glm::orientedAngle(frontVec, desiredFront, axis);
	auto frontBefore = transform.Front();
	transform.RotateRadians(angle, axis);
	auto frontAfter = transform.Front();

	axis = glm::normalize(glm::cross(transform.Front(), glm::vec3(0,1,0)));
	glm::mat4 rotMat = glm::rotate(glm::pi<float>()/2.0f, axis);
	glm::vec3 desiredUp = rotMat * glm::vec4(transform.Front(), 1.0f);

	float angle2 = glm::orientedAngle(transform.Up(), desiredUp, transform.Front());
	auto frontBefore2 = transform.Front();
	transform.RotateRadians(angle2, transform.Front());
	auto frontAfter2 = transform.Front();
}


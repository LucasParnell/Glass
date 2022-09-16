#pragma once
#include "IComponent.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct Camera : IComponent {

	_declspec(dllexport) _cdecl Camera(Transform* position);

	glm::mat4 GetView();


	_declspec(dllexport) void _cdecl MakeMainCamera();
	static Camera* GetMainCamera();

	__declspec(dllexport) void _cdecl updateVectors();
	Transform* transform;

	inline static Camera* mainCamera = nullptr;

	glm::vec3 front = glm::vec3(0.0f);
	glm::vec3 right = glm::vec3(0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f);
	float yaw = 0;
	float pitch = 0;
};
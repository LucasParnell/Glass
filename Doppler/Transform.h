#pragma once
#include "IComponent.h"
#include "glm/vec3.hpp"
class Transform : public IComponent {
public:
	glm::vec3 position = glm::vec3(0.0f);

	Transform() {componentType = TRANSFORM;}

};
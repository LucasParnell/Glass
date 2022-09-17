#pragma once

#include "IComponent.h"
#include "glm/vec3.hpp"

struct Transform : public IComponent {
    Transform() { componentType = TRANSFORM; }

    glm::vec3 position = glm::vec3(0.0f);


};
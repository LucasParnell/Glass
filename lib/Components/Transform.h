#pragma once

#include "IComponent.h"
#include "glm/vec3.hpp"

namespace Components {
    struct Transform : public IComponent {

        glm::vec3 position = glm::vec3(0.0f);
        GLASS_REFLECTABLE();
    };
}
REFL_AUTO(type(Components::Transform, bases<Components::IComponent>))

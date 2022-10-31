#pragma once

#include "IComponent.h"
#include "glm/vec3.hpp"
#include "Components/IComponentFactory/DerivedRegister.h"


namespace Components {
    struct Transform : public IComponent {
        Base::Result SetMembers(std::vector<std::string> list) override;

        glm::vec3 position;

        GLASS_REFLECTABLE();
        REGISTER_DEC_TYPE(Transform);
    };
}
REFL_AUTO(type(Components::Transform, bases < Components::IComponent > ))

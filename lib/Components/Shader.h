#pragma once
#include <cstdint>
#include <vector>
#include "Components/IComponent.h"
#include "Components/Transform.h"
#include "Base/Logging.h"
namespace Components {
    struct Shader : public Components::IComponent {

        uint32_t ID = 1;
        uint32_t other = 3;
        GLASS_REFLECTABLE();

    };
}
REFL_AUTO(type(Components::Shader, bases<Components::IComponent>))
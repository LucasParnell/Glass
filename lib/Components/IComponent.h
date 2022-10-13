#pragma once
#include "refl.hpp"
#include "Base/Reflectable.h"
#include <memory>

namespace Components {
    struct IComponent : public Reflectable {

        GLASS_REFLECTABLE();

        virtual ~IComponent() = default;
    };


}
REFL_AUTO(type(Components::IComponent))
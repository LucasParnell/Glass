#pragma once

#include <string>
#include "glad/glad.h"
#include "lodepng/lodepng.h"
#include "Components/IComponentFactory/DerivedRegister.h"

#include "IComponent.h"

namespace Components {
    struct Texture : public IComponent {
        Base::Result SetMembers(std::vector<std::string> list) override;
        bool loaded = false;
        GLuint ID;
        GLASS_REFLECTABLE();
        REGISTER_DEC_TYPE(Texture);
    };
}

REFL_AUTO(type(Components::Texture, bases<Components::IComponent>))
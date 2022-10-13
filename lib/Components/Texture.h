#pragma once

#include <string>
#include "glad/glad.h"
#include "lodepng/lodepng.h"

#include "IComponent.h"

namespace Components {
    struct Texture : IComponent {
        bool loaded = false;
        GLuint ID;
        GLASS_REFLECTABLE();
    };
}

REFL_AUTO(type(Components::Texture, bases<Components::IComponent>))
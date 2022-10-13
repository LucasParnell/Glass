#pragma once

#include "Components/Texture.h"
#include "Base/Logging.h"
#include "Base/Window.h"

namespace Loaders {
    class TextureLoader {
    public:
        __declspec(dllexport) Components::Texture _cdecl teCreateTexture(const char *texDir);


    };
}

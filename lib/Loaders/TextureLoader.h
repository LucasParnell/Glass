#pragma once

#include "Components/Texture.h"
#include "Base/Logging.h"
#include "Base/Window.h"
#include "Filesystem/Vfs.h"

namespace Loaders {
    class TextureLoader {
    public:
        static __declspec(dllexport) Base::Result _cdecl
        teCreateTexture(Components::Texture
        &texture,
        const std::string &mountpoint,
        const std::string &filename
        );

    private:
        inline static std::unordered_map<std::string, uint32_t> mountedTextures;

    };
}

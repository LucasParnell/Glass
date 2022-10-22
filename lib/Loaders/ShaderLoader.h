#pragma once



#include "glass_export.h"
#include "Base/Result.h"
#include "Components/Mesh.h"
#include "Components/Shader.h"


#include "Rendering/RenderSystem.h"
#include <string>
#include "Filesystem/Vfs.h"

namespace Rendering {
    class RenderSystem;
}

namespace Loaders {
    class ShaderLoader {
    public:
        static GLASS_EXPORT Base::Result
        LoadObject(Components::Shader &shader, Components::Mesh &mesh, Rendering::RenderSystem &renderSystem);
    };
}
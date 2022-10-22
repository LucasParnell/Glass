#pragma once
#include <cstdint>
#include <vector>
#include "Components/IComponent.h"
#include "Components/Transform.h"
#include "Base/Logging.h"
#include "Mesh.h"
#include "Components/IComponentFactory/DerivedRegister.h"

namespace Components {
    struct Shader : public Components::IComponent {
        Base::Result SetMembers(std::vector<std::string> list) override;

        std::string mountpoint;
        std::string shaderName;

        Mesh* meshToLoad;
        uint32_t ID = 1;
        uint32_t other = 3;
        GLASS_REFLECTABLE();
        REGISTER_DEC_TYPE(Shader);
    };
}
REFL_AUTO(type(Components::Shader, bases<Components::IComponent>))
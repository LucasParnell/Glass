#pragma once

#include <cstdint>
#include <vector>
#include "Components/IComponent.h"
#include "Components/Transform.h"

struct Shader : public IComponent {
    Shader() {
        componentType = SHADER;
    }

    uint32_t ID;

};
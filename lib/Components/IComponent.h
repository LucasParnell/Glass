#pragma once

#include <memory>

struct IComponent {
    enum ComponentType {
        METADATA,
        TRANSFORM,
        MESH,
        SHADER,
        TEXTURE,
        CAMERA
    };

    ComponentType componentType;

    IComponent() = default;

    virtual ~IComponent() = default;;
};

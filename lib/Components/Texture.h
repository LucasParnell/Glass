#pragma once

#include <string>
#include "glad/glad.h"
#include "lodepng/lodepng.h"

#include "IComponent.h"

struct Texture : IComponent {
    Texture() {
        this->componentType = TEXTURE;
    }

    bool loaded = false;
    GLuint ID;

    Texture teCreateMesh(const char *texDir);
};
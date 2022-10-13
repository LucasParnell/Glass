#pragma once

#include <vector>
#include "IComponent.h"
#include "glad/glad.h"
#include "tinygltf/tiny_gltf.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Components {
    struct Mesh : IComponent { //Make this easier to use

        tinygltf::Model model;
        std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos;
        GLASS_REFLECTABLE();

    };
}

REFL_AUTO(type(Components::Mesh, bases<Components::IComponent>))
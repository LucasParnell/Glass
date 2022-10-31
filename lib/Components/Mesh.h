#pragma once

#include <vector>
#include "IComponent.h"
#include "glad/glad.h"
#include "tinygltf/tiny_gltf.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Components/IComponentFactory/DerivedRegister.h"

namespace Components {
    struct Mesh : public IComponent { //Make this easier to use
        Base::Result SetMembers(std::vector<std::string> list) override;

        tinygltf::Model model;
        std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos;

        GLASS_REFLECTABLE();
        REGISTER_DEC_TYPE(Mesh);
    };


}

REFL_AUTO(type(Components::Mesh, bases < Components::IComponent > ))
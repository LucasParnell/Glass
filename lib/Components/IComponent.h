#pragma once

#include "refl.hpp"
#include "Base/Reflectable.h"
#include "Base/Result.h"
#include <string>
#include <vector>
#include <memory>
#include <map>

#define REGISTER_DEC_TYPE(NAME) \
    static __declspec(dllexport) DerivedRegister<NAME> reg

#define REGISTER_DEF_TYPE(NAME) \
    DerivedRegister<NAME> NAME::reg(#NAME)


namespace Components {
    struct IComponent : public Reflectable {

        GLASS_REFLECTABLE();

        virtual Base::Result SetMembers(std::vector<std::string> list) = 0;

        virtual ~IComponent() = default;

    };
}

REFL_AUTO(type(Components::IComponent))
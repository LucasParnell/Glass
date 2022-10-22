#pragma once
#include "Components/IComponent.h"
#include <utility>
#include <map>

using Components::IComponent;



struct IComponentFactory {
    typedef std::map<std::string, IComponent*(*)()> map_type;

    static __declspec(dllexport) IComponent *  createInstance(std::string const& s);

    inline static map_type * map = new map_type;
};


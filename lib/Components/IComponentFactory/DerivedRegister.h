#pragma once

#include "IComponentFactory.h"

template<typename T>
IComponent *createT() { return new T; }

template<typename T>
struct DerivedRegister : IComponentFactory {
    __declspec(dllexport) DerivedRegister(std::string const &s) {
        IComponentFactory::map->insert(std::make_pair(s, &createT<T>));
    }
};
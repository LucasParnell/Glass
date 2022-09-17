#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Components/IComponent.h"
#include "Debug/Logging.h"

using Entity = uint32_t;
using Component = uint32_t;

struct ComponentManager {

    //Array of components
    std::vector<IComponent *> components;

    //Entity-Components vector
    std::map<Entity, std::vector<Component>> entityComponents;

    ComponentManager() {
        components.reserve(2048);
    }

    template<class iComponent>
    __declspec(dllexport)  Component __cdecl RegisterComponent() {
        Component component = (Component) components.size();

        components.push_back(new iComponent());
        return component;
    }

    __declspec(dllexport) void __cdecl UnRegisterComponent(Component component) {
        //Add actual removal system, stop being lazy.
        delete components.at(component);
    }

    __declspec(dllexport) void __cdecl AddComponent(Entity entity, Component component) {
        entityComponents[entity].push_back(component);
    }

    __declspec(dllexport) IComponent *__cdecl GetComponentPtr(Component component) {
        auto ptr = components.at(component);
        return (ptr);
    }

    __declspec(dllexport) IComponent *__cdecl SetComponent(Component componentID, IComponent *componentObj) {
        components.at(componentID) = componentObj;

        return components.at(componentID);
    }

};
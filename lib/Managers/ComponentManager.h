#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Components/IComponent.h"
#include "Debug/Logging.h"
#include "Components/IComponentFactory/IComponentFactory.h"


using Entity = std::string;
using Component = uint32_t;



namespace Managers {
    struct ComponentManager {

        //Array of components

        std::vector<Components::IComponent *> components;

        //Entity-Components vector
        std::map<Entity, std::vector<Component>> entityComponents;

        ComponentManager() {
            components.reserve(2048);
        }

        __declspec(dllexport)  Component __cdecl RegisterComponent(std::string typeName) {
            auto component = (Component) components.size();
            auto obj = IComponentFactory::createInstance(typeName);
            components.push_back(obj);
            return component;
        }

        __declspec(dllexport) void __cdecl UnRegisterComponent(Component component) {
            if(components.size()>component) {
                components.erase(components.begin() + component);
            }
            else{
                MLOG(LOG_WARN, "Component doesn't exist");
            }
        }

        __declspec(dllexport) void __cdecl RemoveEntity(Entity entity) {
            //Remove all components
            auto &component_list = entityComponents[entity];


            std::vector<uint32_t> shift_buffer;



            for(uint32_t i=component_list.size()-1; i>0; i--) {
                UnRegisterComponent(component_list[i]);
                shift_buffer.push_back(component_list[i]);
            }

            entityComponents.erase(entity);

            //Could be optimised
            for(auto &ec_pair : entityComponents){
                for(auto &comp : ec_pair.second){
                    for(auto &shift_index : shift_buffer)
                        if(comp > shift_index)
                            comp--;
                }
            }


        }

        __declspec(dllexport) void __cdecl AddComponent(Entity entity, Component component) {
            entityComponents[entity].push_back(component);
        }

        __declspec(dllexport) Components::IComponent *__cdecl GetComponentPtr(Component component) {
            auto ptr = components.at(component);
            return (ptr);
        }

        __declspec(dllexport) Components::IComponent *
        __cdecl SetComponent(Component componentID, Components::IComponent *componentObj) {
            components.at(componentID) = componentObj;

            return components.at(componentID);
        }

    };
}
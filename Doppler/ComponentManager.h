#pragma once
#include <string>
#include <vector>
#include <memory>
#include "IComponent.h"

using Entity = uint32_t;
using Component = uint32_t;

struct ComponentManager {

	//Array of components
	std::vector<IComponent*> components;

	std::vector<std::pair<Entity, Component>> entityComponents;

	template<class iComponent>
	Component RegisterComponent() {
		int component = components.size();

		components.push_back(new iComponent());
		return component;
	}

	void UnRegisterComponent(Component component) {
		//Add actual remove system, please :(
		components.at(component) = new IComponent();
	}

	void AddComponent(Entity entity, Component component) {
		entityComponents.push_back({ entity, component });
	}

	IComponent* GetComponentPtr(Component component) {
		return components.at(component);
	}


};
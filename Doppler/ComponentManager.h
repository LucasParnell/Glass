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

	//Entity-Components vector
	std::vector<std::pair<Entity, Component>> entityComponents;

	
	template<class iComponent>
	Component RegisterComponent() {
		int component = components.size();

		components.push_back(new iComponent());
		return component;
	}

	void UnRegisterComponent(Component component) {
		//Add actual removal system, stop being lazy.
		components.at(component) = new IComponent();
	}

	void AddComponent(Entity entity, Component component) {
		entityComponents.push_back({ entity, component });
	}

	std::shared_ptr<IComponent> GetComponentPtr(Component component) const {
		auto ptr = components.at(component);
		return std::shared_ptr<IComponent>(ptr);
	}

	Entity GetParent(Component component){
		//Slow, can optimise later
		//Might swap entityComponents for an ordered map, could then implement Binary Search which is O(log n)
		for (int i = 0; i < entityComponents.size(); i++) {
			if (entityComponents[i].second == component) {
				return entityComponents[i].first;
			}
		}
	}


};
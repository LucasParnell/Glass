#pragma once
#include <string>
#include <vector>
#include <memory>

using Entity = uint32_t;
using Component = uint32_t;

struct EntityManager {

	//Array of IDs
	std::vector<Entity> entities;

	EntityManager() {
		entities.reserve(2056);
	}

	//Functions
	__declspec(dllexport) Entity __cdecl CreateEntity();



};
#include "pch.h"
#include "EntityManager.h"
#include "Logging.h"

Entity EntityManager::CreateEntity() {
	Entity id = entities.size();
	entities.push_back(id);
	return id;
}
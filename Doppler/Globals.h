#pragma once
#include "ComponentManager.h"
#include "EntityManager.h"
#include "Logging.h"

struct Globals {
	static ComponentManager* mComponentManager;
	static EntityManager* mEntityManager;
};
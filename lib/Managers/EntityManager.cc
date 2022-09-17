#include "EntityManager.h"
#include "Debug/Logging.h"

Entity EntityManager::CreateEntity() {
    Entity id = entities.size();
    entities.push_back(id);
    return id;
}
#include <cassert>
#include "EntityManager.h"

using namespace Managers;
Entity EntityManager::CreateEntity() {
    UUIDv4::UUID uuid = uuidGenerator.getUUID();
    std::string s = uuid.str();
    entities.push_back(s);
    MLOG(LOG_INFO, s);
    return s;
}
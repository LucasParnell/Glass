#pragma once

#include <string>
#include <vector>
#include <memory>
#include "uuid/uuid_v4.h"

#include "Base/Logging.h"

using Entity = std::string;
using Component = uint32_t;

namespace Managers {
    class EntityManager {
    public:
        //Array of IDs
        std::vector<Entity> entities;

        EntityManager() {

            entities.reserve(2056);
        }

        //Functions
        __declspec(dllexport) Entity __cdecl CreateEntity();

    private:
        UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;

    };
}
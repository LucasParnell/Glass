#pragma once

#include <string>
#include <vector>
#include <memory>
#include "uuid/uuid_v4.h"
#include "Filesystem/VFS.h"
#include "tinygltf/json.hpp"
#include "refl.hpp"
#include "Base/Logging.h"
#include "ComponentManager.h"
#include "Components/Camera.h"
#include "Rendering/RenderSystem.h"


using Entity = std::string;
using Component = uint32_t;

namespace Rendering {
    class RenderSystem;
}

namespace Managers {
    class EntityManager {
    public:
        //Array of IDs
        std::vector<Entity> entities;

        EntityManager() {
            entities.reserve(2056);
        }

        //Functions
        Entity GLASS_EXPORT CreateEntity();

        void GLASS_EXPORT ConstructGSD(std::string mountPoint, std::string filename, ComponentManager* compMan, Rendering::RenderSystem &renderSystem);

    private:
        UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;

    };
}
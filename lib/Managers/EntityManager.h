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
#include "Components/Mesh.h"
#include "ComponentManager.h"
#include "Components/IComponentFactory/IComponentFactory.h"
#include "Components/Camera.h"
#include "Components/Shader.h"

using Entity = std::string;
using Component = uint32_t;

namespace Rendering {
    class RenderSystem;
}

namespace Managers {
    class EntityManager {
    public:
        //Array of IDs
        std::vector<std::pair<Entity, std::string>> entities;

        EntityManager() {
            entities.reserve(2056);
        }

        //Functions
        Entity GLASS_EXPORT CreateEntity(std::string readable);

        void GLASS_EXPORT RemoveEntity(Entity uuid);


        void GLASS_EXPORT ConstructGSD(std::string mountPoint, std::string filename, ComponentManager *compMan,
                                       Rendering::RenderSystem &renderSystem);

    private:
        UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;

    };
}
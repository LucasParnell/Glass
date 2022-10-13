
#pragma once

#include "glass_export.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <cstdint>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Result.h"
#include "Window.h"
#include "Managers/EntityManager.h"
#include "Managers/ComponentManager.h"
#include "Rendering/RenderSystem.h"
#include "Logging.h"

namespace Base {

    class Glass {
    public:

        GLASS_EXPORT Glass();

        GLASS_EXPORT ~Glass();

        //Create an OpenGL context with given parameters
        GLASS_EXPORT Result iCreateWindow(Window &window);

        GLASS_EXPORT Result iBeginEngineLoop();

        GLASS_EXPORT Result iSerialiseEntity(const Entity &entity);

        GLASS_EXPORT Managers::ComponentManager *pGetComponentManager();

        GLASS_EXPORT Managers::EntityManager *pGetEntityManager();

        GLASS_EXPORT Rendering::RenderSystem *pGetRenderSystem();

    private:

        Result iCleanup();

        Managers::ComponentManager mComponentManager;
        Managers::EntityManager mEntityManager;
        Rendering::RenderSystem mRenderSystem;
        Window mWindow{};

    };
}
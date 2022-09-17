
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
#include "Debug/Logging.h"

class Glass {
public:

    GLASS_EXPORT Glass();

    GLASS_EXPORT ~Glass();

    //Create an OpenGL context with given parameters
    GLASS_EXPORT Result iCreateWindow(Window &window);

    GLASS_EXPORT Result iBeginEngineLoop();

    GLASS_EXPORT ComponentManager * pGetComponentManager();

    GLASS_EXPORT EntityManager * pGetEntityManager();

    GLASS_EXPORT RenderSystem * pGetRenderSystem();

    GLASS_EXPORT Window * pGetWindow();

private:

    Result iCleanup();

    ComponentManager mComponentManager;
    EntityManager mEntityManager;
    RenderSystem mRenderSystem;
    Window mWindow;

};
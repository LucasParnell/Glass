#pragma once
#include "glass_export.h"
//Temporary
#include "imgui.h"
#include "Managers/EntityManager.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Base/Window.h"
#include "LuaJIT/lua.h"
#include "Debug/Logging.h"


namespace GUI {
class GUIHandler {
public:
    static void Initialise(GLFWwindow* window);
    static void Cleanup();
    static void BeginFrame();
    static void Demo();
    static void EndFrame();

    static void EntityUI(Managers::EntityManager& entityManager, Managers::ComponentManager& componentManager);
    static void InspectorUI(Managers::EntityManager& entityManager, Managers::ComponentManager& componentManager);
    static void LuaConsoleUI(lua_State* L);
private:
    inline static bool inspectorOpen = false;
    inline static Entity inspectorEntity = "";

};
}


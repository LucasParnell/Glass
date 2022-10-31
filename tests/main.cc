// TestWindow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Fix includes and stuff, am very tired today
#include "Base/Glass.h"
#include "Loaders/TextureLoader.h"
#include "Filesystem/Vfs.h"
#include "Base/LuaDriver.h"
#include "GUI/GuiHandler.h"

using Filesystem::VFS;

int main() {

    Base::Glass engine;
    Base::Window window{};
    Base::LuaDriver luaDriver;
    window.title = "Glass Window";
    window.width = 1920;
    window.height = 1080;
    window.fullscreenType = Base::Window::FullscreenType::Disabled;

    engine.iCreateWindow(window);
    MLOG(LOG_INFO, "Window Created");


    auto entityMan = engine.pGetEntityManager();
    auto compMan = engine.pGetComponentManager();
    auto renderer = engine.pGetRenderSystem();
    MLOG(LOG_INFO, "Retrieved Managers");


    VFS::Mount("data\\test.gpk");
    VFS::ListAll();

    entityMan->ConstructGSD("GamePak:\\scenes\\", "demo_scene.gsd", compMan, *renderer);

    luaDriver.BeginState();


    engine.iBeginEngineLoop();
}
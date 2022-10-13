// TestWindow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Fix includes and stuff, am very tired today
#include "../lib/Base/Glass.h"
#include "Loaders/TextureLoader.h"
#include "Filesystem/Vfs.h"

int main() {

    Base::Glass engine;
    Base::Window window{};
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

    //Make camera
    auto camera = entityMan->CreateEntity();
    auto camera_transform = compMan->RegisterComponent<Components::Transform>();
    auto cam_transform_ptr = (Components::Transform *) (compMan->GetComponentPtr(camera_transform));
    auto lCamera = new Components::Camera(cam_transform_ptr);

    cam_transform_ptr->position.z -= 1.0f;
    lCamera->UpdateVectors();

    //Make monkey
    auto monkey = entityMan->CreateEntity();

    //Register components
    auto monkey_transform = compMan->RegisterComponent<Components::Transform>();
    auto monkey_mesh = compMan->RegisterComponent<Components::Mesh>();
    auto monkey_shader = compMan->RegisterComponent<Components::Shader>();
    auto monkey_texture = compMan->RegisterComponent<Components::Texture>();

    //Add components to monkey
    compMan->AddComponent(monkey, monkey_transform);
    compMan->AddComponent(monkey, monkey_mesh);
    compMan->AddComponent(monkey, monkey_shader);
    compMan->AddComponent(monkey, monkey_texture);

    //Load mesh and shader
    Loaders::MeshLoader ml;
    Loaders::TextureLoader tl;
    Components::Mesh monkey_lMesh = ml.meCreateMesh("table");
    Components::Texture monkey_lTexture = tl.teCreateTexture("CoolWorld/rock.png");

    //TODO(Make Shader Loader):
    Components::Shader monkey_lShader = renderer->LoadObject(&monkey_lMesh, "monkey");


    //Store actual objects
    compMan->SetComponent(monkey_mesh, &monkey_lMesh);
    compMan->SetComponent(monkey_shader, &monkey_lShader);
    compMan->SetComponent(monkey_texture, &monkey_lTexture);


    engine.iBeginEngineLoop();
}
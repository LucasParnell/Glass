// TestWindow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Fix includes and stuff, am very tired today
#include "../lib/Base/Glass.h"
#include "../lib/Components/Mesh.h"
#include "../lib/Loaders/MeshLoader.h"
//#include "Renderer.h"
#include "../lib/Components/Camera.h"
#include "../lib/Components/Texture.h"
#include "Loaders/TextureLoader.h"

int main()
{

    Glass engine;
    Window window{};
    window.title = "Glass Window";
    window.width = 1920;
    window.height = 1080;
    window.fullscreenType = Window::FullscreenType::kDisabled;

    engine.iCreateWindow(window);



    auto entityMan = engine.pGetEntityManager();
    auto compMan = engine.pGetComponentManager();
    auto renderer = engine.pGetRenderSystem();

    //Make camera
    auto camera = entityMan->CreateEntity();
    auto camera_transform = compMan->RegisterComponent<Transform>();
    auto cam_transform_ptr = (Transform*)(compMan->GetComponentPtr(camera_transform));
    auto lCamera = new Camera(cam_transform_ptr);

    cam_transform_ptr->position.z -= 1.0f;
    lCamera->UpdateVectors();

    //Make monkey
    auto monkey = entityMan->CreateEntity();

    //Register components
    auto monkey_transform = compMan->RegisterComponent<Transform>();
    auto monkey_mesh = compMan->RegisterComponent<Mesh>();
    auto monkey_shader = compMan->RegisterComponent<Shader>();
    auto monkey_texture = compMan->RegisterComponent<Texture>();

    //Add components to monkey
    compMan->AddComponent(monkey, monkey_transform);
    compMan->AddComponent(monkey, monkey_mesh);
    compMan->AddComponent(monkey, monkey_shader);
    compMan->AddComponent(monkey, monkey_texture);

    //Load mesh and shader
    MeshLoader ml;
    TextureLoader tl;
    Mesh monkey_lMesh = ml.meCreateMesh("table");
    Texture monkey_lTexture = tl.teCreateTexture("CoolWorld/rock.png");

    //TODO(Make Shader Loader):
    Shader monkey_lShader = renderer->LoadObject(&monkey_lMesh, "monkey");


    //Store actual objects
    compMan->SetComponent(monkey_mesh, &monkey_lMesh);
    compMan->SetComponent(monkey_shader, &monkey_lShader);
    compMan->SetComponent(monkey_texture, &monkey_lTexture);

    engine.iBeginEngineLoop();
}
// TestWindow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Doppler.h"
#include "Mesh.h"
#include "MeshLoader.h"
//#include "Renderer.h"
#include "Camera.h"
#include "Texture.h"
int main()
{

    Doppler engine;
    engine.CreateEngineInstance();



    auto entityMan = engine.GetEntityManager();
    auto compMan = engine.GetComponentManager();
    auto renderer = engine.GetRenderer();

    //Make camera
    auto camera = entityMan->CreateEntity();
    auto camera_transform = compMan->RegisterComponent<Transform>();
    auto cam_transform_ptr = (Transform*)(compMan->GetComponentPtr(camera_transform));
    auto lCamera = new Camera(cam_transform_ptr);

    cam_transform_ptr->position.z -= 1.0f;
    lCamera->updateVectors();

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

    Mesh monkey_lMesh = ml.CreateMesh("table");
    std::cout << "test";
    Shader monkey_lShader = renderer->LoadObject(&monkey_lMesh, "monkey");
    Texture* monkey_lTexture = new Texture();
    monkey_lTexture->Load("CoolWorld/rock.png");

    //Store actual objects
    compMan->SetComponent(monkey_mesh, &monkey_lMesh);
    compMan->SetComponent(monkey_shader, &monkey_lShader);
    compMan->SetComponent(monkey_texture, monkey_lTexture);

   
    
    

    //TODO:
    // TEXTURES
    // 
    // MAKE MEMORY CONTIGUOUS IN VECTORS
    //OPTIMISE SEARCH CODE
    engine.BeginEngineLoop();
    engine.Cleanup();
}
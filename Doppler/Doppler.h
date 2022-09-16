#pragma once
#include <cstdint>
#include <iostream>
#include "EntityManager.h"
#include "Transform.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Globals.h"
#include "Metadata.h"
#include "glm/vec3.hpp"


struct Doppler {
	//Initialisation Things
	 __declspec(dllexport) uint32_t __cdecl CreateEngineInstance();
	 __declspec(dllexport) GLFWwindow* __cdecl CreateViewport(int winX, int winY, std::string winTitle);

	 __declspec(dllexport) EntityManager* __cdecl GetEntityManager();
	 __declspec(dllexport) ComponentManager* __cdecl GetComponentManager();
	 __declspec(dllexport) Renderer* __cdecl GetRenderer();

	 __declspec(dllexport) uint32_t __cdecl BeginEngineLoop();

	 __declspec(dllexport) uint32_t __cdecl Cleanup();

	
};
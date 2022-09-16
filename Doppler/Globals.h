#pragma once
#include "ComponentManager.h"
#include "EntityManager.h"
#include "Logging.h"
#include "Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct Globals {
	static ComponentManager* mComponentManager;
	static EntityManager* mEntityManager;
	static GLFWwindow* mViewport;
	static Renderer* mRenderer;
	inline static int SCR_WIDTH;
	inline static int SCR_HEIGHT;
};
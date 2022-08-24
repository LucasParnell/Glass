#include "pch.h"
#include "Doppler.h"
#include "Renderer.h"


//Using macros because i cant pass types to a template in runtime (obviously)
#define DPCAST(Type, Args) std::dynamic_pointer_cast<Type>(Args)
#define GETCOMPONENT(Type, ID) DPCAST(Type, Globals::mComponentManager->GetComponentPtr(ID));

//GLFW is kinda goofy
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


GLFWwindow* Globals::mViewport = nullptr;

uint32_t Doppler::CreateEngineInstance() {

    MLOG(LOG_INFO, "Setting up");
    Globals::mComponentManager = new ComponentManager();
    Globals::mEntityManager = new EntityManager();

	MLOG(LOG_INFO, "Creating OpenGL context");
    Globals::mViewport = CreateViewport(800, 600, "Engine");

    MLOG(LOG_INFO, "Window created successfully :)");

	return 0;
}

uint32_t Doppler::BeginEngineLoop() {

    auto window = &Globals::mViewport;
    auto renderer = new Renderer();

    auto testShader = renderer->MakeDebugTriangle();

    while (!glfwWindowShouldClose(*window))
    {
        // input
        // -----
        if (glfwGetKey(*window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(*window, true);




        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer->Render(testShader);
        //swap buffers, poll IO events
        
        glfwSwapBuffers(*window);
        glfwPollEvents();
    }
    return 0;
}


uint32_t Doppler::Cleanup() {
    glfwTerminate();
    return 0;
}

GLFWwindow* Doppler::CreateViewport(int winX, int winY, std::string winTitle)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(winX, winY, winTitle.c_str(), NULL, NULL);
    if (window == NULL)
    {
        MLOG(LOG_ERROR, "Could not create window");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        MLOG(LOG_ERROR, "Could not initialize GLAD");
        return nullptr;
    }


    return window;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

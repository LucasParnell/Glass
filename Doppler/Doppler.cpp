#include "pch.h"
#include "Doppler.h"
#include "MeshLoader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//Using macros because i cant pass types to a template in runtime (obviously)
#define DPCAST(Type, Args) std::dynamic_pointer_cast<Type>(Args)
#define GETCOMPONENT(Type, ID) DPCAST(Type, Globals::mComponentManager->GetComponentPtr(ID));

//GLFW is kinda goofy
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

GLFWwindow* Globals::mViewport = nullptr;

uint32_t Doppler::CreateEngineInstance() {

    MLOG(LOG_INFO, "Setting up");
	MLOG(LOG_INFO, "Creating OpenGL context");
    Globals::mViewport = CreateViewport(2560, 1440, "Engine");
   

    MLOG(LOG_INFO, "Window created successfully :)");

    //glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(Globals::mViewport, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(Globals::mViewport, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	return 0;
}

uint32_t Doppler::BeginEngineLoop() {

    auto window = &Globals::mViewport;
    glfwSetCursorPosCallback(*window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    double currentTime = 0;
    double previousTime = 0;
    double timeDelta = currentTime - previousTime;

    while (!glfwWindowShouldClose(*window))
    {
        currentTime = glfwGetTime();
        double timeDelta = (currentTime - previousTime);
        previousTime = glfwGetTime();

        // input
        // -----
        if (glfwGetKey(*window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(*window, true);



        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
  

        //FOR DEBUG
        auto camera = Camera::GetMainCamera();
        const float cameraSpeed = 2 * timeDelta; // adjust accordingly
        if (glfwGetKey(*window, GLFW_KEY_W) == GLFW_PRESS)
            camera->transform->position += cameraSpeed * camera->front;
        if (glfwGetKey(*window, GLFW_KEY_S) == GLFW_PRESS)
            camera->transform->position -= cameraSpeed * camera->front;
        if (glfwGetKey(*window, GLFW_KEY_A) == GLFW_PRESS)
            camera->transform->position -= glm::normalize(glm::cross(camera->front, camera->up)) * cameraSpeed;
        if (glfwGetKey(*window, GLFW_KEY_D) == GLFW_PRESS) 
            camera->transform->position += glm::normalize(glm::cross(camera->front, camera->up)) * cameraSpeed;




        camera->updateVectors();
        //-----------------

        
        Globals::mRenderer->Render();



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
    auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    winX = mode->width;
    winY = mode->height;
    Globals::SCR_WIDTH = winX;
    Globals::SCR_HEIGHT = winY;
    int count = 2;
    GLFWwindow* window = glfwCreateWindow(winX, winY, winTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
    if (window == NULL)
    {
        MLOG(LOG_ERROR, "Could not create window");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        MLOG(LOG_ERROR, "Could not initialize GLAD");
        return nullptr;
    }


    return window;
}

EntityManager* __cdecl Doppler::GetEntityManager()
{
    return Globals::mEntityManager;
}

ComponentManager* __cdecl Doppler::GetComponentManager()
{
    return Globals::mComponentManager;
}

Renderer* __cdecl Doppler::GetRenderer()
{
    return Globals::mRenderer;
}

auto lastX = 2560 / 2;
auto lastY = 1440 / 2;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    auto camera = Camera::GetMainCamera();
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    if (camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if (camera->pitch < -89.0f)
        camera->pitch = -89.0f;
}
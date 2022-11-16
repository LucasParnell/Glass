#include "Glass.h"
#include "Components/Camera.h"

//GLFW is kinda goofy
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool cursor_focus = true;


using Base::Glass;

Glass::Glass() = default;

Glass::~Glass() {
    Result status = iCleanup();
    if (status != Result::STATUS_OK)
        MLOG(LOG_WARN, "There was an error during cleanup");
}

Base::Result Glass::iCleanup() {
    glfwTerminate();
    GUI::GUIHandler::Cleanup();
    return Result::STATUS_OK;
}


Base::Result Glass::iCreateWindow(Window &window) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = nullptr;
    int width = window.width;
    int height = window.height;

    switch (window.fullscreenType) {
        case Window::Enabled: {
            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            width = mode->width;
            height = mode->height;
            break;
        }
        case Window::Borderless: {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            width = mode->width;
            height = mode->height + 1;
            monitor = nullptr;
            break;
        }
        case Window::MaximizedWindow: {
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
            break;
        }

        default:
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            break;

    }

    //Create a new window with the supplied parameters
    GLFWwindow *sWindow = glfwCreateWindow(width, height, window.title, monitor, nullptr);


    if (sWindow == nullptr) {
        MLOG(LOG_ERROR, "Could not create window");
        glfwTerminate();
        return Result::STATUS_ERROR;
    }

    glfwMakeContextCurrent(sWindow);

    glfwSwapInterval(0);
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        MLOG(LOG_ERROR, "Could not initialize GLAD");
        return Result::STATUS_ERROR;
    }

    window.SetWindow(sWindow);
    mWindow = window;

    mRenderSystem.SetWindow(&window);

    return Result::STATUS_OK;
}

Base::Result Glass::iBeginEngineLoop() {
    //Set up engine for loop

    GLFWwindow *pWindow = mWindow.pGetWindow();



    luaDriver.BeginState();


    //Set up mouse
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(pWindow, mouse_callback);
    glfwSetKeyCallback(pWindow, key_callback);

    GUI::GUIHandler::Initialise(pWindow );
    Utils::SetupImGuiStyle(true, 1.0f);
    //Enable Depth Testing and Culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    //Setup frame-time counter
    double currentTime;
    double previousTime = 0;
    double timeDelta;

    while (!glfwWindowShouldClose(pWindow)) {
        glClearColor((float) ((1.0 / 255) * 0), (float) ((1.0 / 255) * 0), (float) ((1.0 / 255) * 0), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        GUI::GUIHandler::BeginFrame();

        currentTime = glfwGetTime();
        timeDelta = (currentTime - previousTime);
        previousTime = glfwGetTime();


        //BELOW IS ALL TEMPORARY AND WILL BE HANDLED IN LUA LATER ON
        // input
        // -----
        Components::Camera *pCamera = Components::Camera::pGetMainCamera();

        if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(pWindow, true);



        if(cursor_focus) {

            const float cameraSpeed = 2.0f * (float) timeDelta; // adjust accordingly
            if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
                pCamera->transform->position += cameraSpeed * pCamera->front;
            if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
                pCamera->transform->position -= cameraSpeed * pCamera->front;
            if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
                pCamera->transform->position -= glm::normalize(glm::cross(pCamera->front, pCamera->up)) * cameraSpeed;
            if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
                pCamera->transform->position += glm::normalize(glm::cross(pCamera->front, pCamera->up)) * cameraSpeed;

        }




        pCamera->UpdateVectors();
        //-----------------



        GUI::GUIHandler::EntityUI(mEntityManager,mComponentManager);
        GUI::GUIHandler::InspectorUI(mEntityManager,mComponentManager);
        GUI::GUIHandler::LuaConsoleUI(luaDriver.luaState);


        mRenderSystem.Render(&mWindow, &mEntityManager, &mComponentManager);
        GUI::GUIHandler::EndFrame();

        glfwSwapBuffers(pWindow);

    }

    return Result::STATUS_OK;
}


Managers::ComponentManager *Glass::pGetComponentManager() {
    return &mComponentManager;
}

Managers::EntityManager *Glass::pGetEntityManager() {
    return &mEntityManager;
}

Rendering::RenderSystem *Glass::pGetRenderSystem() {
    return &mRenderSystem;
}

//Replace this
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

}

//Again, is temporary
auto lastX = 2560 / 2;
auto lastY = 1440 / 2;
bool firstMouse = true;

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    auto camera = Components::Camera::pGetMainCamera();
    if (firstMouse) {
        lastX = (int) xpos;
        lastY = (int) ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if(cursor_focus) {
        camera->yaw += xoffset;
        camera->pitch += yoffset;

        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_I && action == GLFW_PRESS) {
        if(cursor_focus) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cursor_focus = !cursor_focus;

    }
}

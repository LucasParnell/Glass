#pragma once
#include "glass_export.h"

#include "GLFW/glfw3.h"


class Window {
public:
     enum FullscreenType {
        kDisabled,
        kEnabled,
        kBorderless
    };

    const char *title;
    int width;
    int height;
    FullscreenType fullscreenType;

    void SetWindow(GLFWwindow *window);

    GLFWwindow *pGetWindow();

private:
    GLFWwindow *window;
};
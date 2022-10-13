#pragma once

#include "glass_export.h"

#include "GLFW/glfw3.h"

namespace Base {
    class Window {
    public:
        enum FullscreenType {
            Disabled,
            Enabled,
            Borderless
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
}
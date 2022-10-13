#include "Window.h"

using Base::Window;

//Setter for private pWindow member
void Window::SetWindow(GLFWwindow *glWindow) {
    this->window = glWindow;
}

//Getter for private pWindow member
GLFWwindow *Window::pGetWindow() {
    return window;
}

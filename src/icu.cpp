#include "icu.hpp"

#include "global.hpp"
#include "util.hpp"

void icu_window_size(GLFWwindow* win, int w, int h) {
    GL(glViewport(0, 0, w, h));
    global.camera.set_aspect(float(w)/h);
}

void icu_key(GLFWwindow* win, int key, int scode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(win, GLFW_TRUE);
                break;
        };
    }
}

void icu_mouse_button(GLFWwindow* win, int button, int action, int mods) {
}

void icu_cursor_pos(GLFWwindow* win, double x_pos, double y_pos) {
}

void icu_scroll(GLFWwindow* win, double x_offset, double y_offset) {
}

#include "icu.hpp"
#include <iostream>

#include <glm/glm.hpp>

#include "global.hpp"
#include "util.hpp"

void apply_orbit_momentum(double x_pos, double y_pos) {
    glm::ivec2 win_size = global.window.size();

    glm::vec3 v0 = arc_ball_mapping(global.mouse.last_pos, win_size);
    glm::vec3 v1 = arc_ball_mapping(glm::vec2(x_pos, y_pos), win_size);

    double angle = acos(std::max(-1.0f, std::min(1.0f, glm::dot(v0, v1))));
    glm::vec3 axis = glm::cross(v1, v0);
    global.camera.orbit(-angle, axis, glm::vec3(0.0f, 0.0f, 0.0f));
}

void applay_drag_offset(double x_pos, double y_pos) {
    glm::ivec2 win_size = global.window.size();
    double distance = glm::length(global.camera.position());

    double x_move = distance * (x_pos - global.mouse.last_pos.x) / win_size.x;
    double y_move = distance * (y_pos - global.mouse.last_pos.y) / win_size.y;

    global.camera.translate_x(x_move);
    global.camera.translate_y(y_move);
}

// callback functions =+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

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
    if (mods == GLFW_MOD_SHIFT && button == GLFW_MOUSE_BUTTON_LEFT) {
        button = GLFW_MOUSE_BUTTON_MIDDLE;
    }

    if (action == GLFW_PRESS) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                global.mouse.LMB_down = true;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                global.mouse.MMB_down = true;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                global.mouse.RMB_down = true;
                break;
        }
    }

    if (action == GLFW_RELEASE) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                global.mouse.LMB_down = false;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                global.mouse.MMB_down = false;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                global.mouse.RMB_down = false;
                break;
        }
    }
}

void icu_cursor_pos(GLFWwindow* win, double x_pos, double y_pos) {
    if (global.mouse.LMB_down) {
        apply_orbit_momentum(x_pos, y_pos);
    }

    if (global.mouse.MMB_down) {
        /* applay_drag_offset(x_pos, y_pos); */
    }

    global.mouse.last_pos = glm::ivec2(x_pos, y_pos);
}

void icu_scroll(GLFWwindow* win, double x_offset, double y_offset) {
}

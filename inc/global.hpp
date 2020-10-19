#pragma once

#include <glm/glm.hpp>

#include "camera.hpp"
#include "window.hpp"

#define NAME "leaphi"
#define WIDTH 960
#define HEIGHT 540
#define ASPECT WIDTH / HEIGHT
#define FOV 30.0f
#define ORBIT_CENTER glm::vec3(0.0f, 0.0f, 0.0f)

struct global_t {
    Window window = Window(NAME, WIDTH, HEIGHT);
    Camera camera = Camera(FOV, ASPECT);

    struct leaf_t {
        float f = 0.0f; // generation factor [-1,1]
        float g = 1.0f; // growth stage [0,1]
    } leaf;

    struct mouse_t {
        bool LMB_down = false;
        bool MMB_down = false;
        bool RMB_down = false;
        glm::ivec2 last_pos;
    } mouse;

    double delta;
};

extern global_t global;

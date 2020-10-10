#pragma once

#include <glm/glm.hpp>

#include "camera.hpp"
#include "window.hpp"

#define NAME "leaphi"
#define WIDTH 960
#define HEIGHT 540
#define ASPECT WIDTH / HEIGHT
#define FOV 45.0f

struct global_t {
    Window window = Window(NAME, WIDTH, HEIGHT);
    Camera camera = Camera(FOV, ASPECT);

    double delta;
};

extern global_t global;

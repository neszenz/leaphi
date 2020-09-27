#pragma once

#include "window.hpp"

struct global_t {
    Window window = Window("leaphi", 960, 540);
    double delta;
};

extern global_t global;

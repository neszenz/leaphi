#include "util.hpp"

#include <thread>
#include <chrono>

void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


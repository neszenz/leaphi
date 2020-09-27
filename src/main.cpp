#include "global.hpp"

#include "util.hpp"

global_t global;

int main(void) {
    while (!global.window.should_close()) {
        global.delta = global.window.update();
        sleep(16);
    }

    return 0;
}

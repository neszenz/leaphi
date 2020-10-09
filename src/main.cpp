#include "global.hpp"

#include "geometry.hpp"
#include "shader.hpp"
#include "util.hpp"

global_t global;

int main(void) {
    Geometry g;
    Shader s("blubb", "res/shaders/basic.vert", "res/shaders/basic.frag");
    glm::mat4 m(1.0f);
    while (!global.window.should_close()) {
        global.delta = global.window.update();
        s.uniformMat4("u_matrix", m);
        s.use();
        g.draw();
        sleep(16);
    }

    return 0;
}

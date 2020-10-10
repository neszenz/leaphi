#include "global.hpp"
#include <GL/glew.h>

#include "geometry.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "util.hpp"

global_t global;

Mesh build() {
    Geometry g;
    Shader s("base", "res/shaders/basic.vert", "res/shaders/basic.frag");
    Mesh triangle(g, s);
    return triangle;
}

void reset_opengl() {
    GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    GL(glEnable(GL_DEPTH_TEST));

    GL(glEnable(GL_CULL_FACE));
    GL(glFrontFace(GL_CCW));
    GL(glCullFace(GL_BACK));

    GL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

int main(void) {
    reset_opengl();
    Mesh triangle = build();
    global.camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    triangle.rotate(glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    while (!global.window.should_close()) {
        global.delta = global.window.update();
        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        triangle.rotate(0.8f*global.delta, glm::vec3(0.8f, 0.6f, -0.4f));
        glm::mat4 p = global.camera.proj_matrix();
        glm::mat4 v = global.camera.view_matrix();
        glm::mat4 m = triangle.model_matrix();
        triangle.shader().uniform("u_matrix", p*v*m);
        triangle.draw();
        sleep(10);
    }

    return 0;
}

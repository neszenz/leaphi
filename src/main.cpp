#include "global.hpp"
#include <GL/glew.h>

#include "color_cube.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "util.hpp"

global_t global;

Mesh build_cube() {
    Color_Cube g;
    Shader s("base", "res/shaders/basic.vert", "res/shaders/basic.frag");
    Mesh mesh(g, s);
    return mesh;
}

void reset_opengl() {
    GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    GL(glEnable(GL_DEPTH_TEST));

    GL(glEnable(GL_CULL_FACE));
    GL(glFrontFace(GL_CCW));
    GL(glCullFace(GL_BACK));

    GL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

void render(const Camera& cam, const Mesh& mesh) {
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    glm::mat4 p = cam.proj_matrix();
    glm::mat4 v = cam.view_matrix();
    glm::mat4 m = mesh.model_matrix();

    mesh.shader().uniform("u_matrix", p*v*m);

    mesh.draw();
}

int main(void) {
    reset_opengl();

    Mesh cube = build_cube();
    global.camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));

    while (!global.window.should_close()) {
        global.delta = global.window.update();

        render(global.camera, cube);

        sleep(10);
    }

    return 0;
}

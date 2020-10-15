#include "global.hpp"
#include <GL/glew.h>

#include <bezier_curve.hpp>
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

Mesh build_leaf() {
    float f = 0.5;
    float l = 16;

    float a = f * (M_PI / l) + (M_PI / 2);
    float b = -a + M_PI;
    float c = a;

    float d = 0.3f;
    float e = 0.2f;

    float size = 1.0f;
    glm::vec2 begin = glm::vec2(0.0f, 0.0f);
    glm::vec2 end = glm::vec2(size, 0.0f);

    float s0_radius = 0.4 * size;
    float s1_radius = 0.8 * size;
    float s2_x_radius = 0.2 * size;
    float s2_y_radius = 0.4 * size;
    glm::vec2 s0 = glm::vec2(s0_radius*sin(a), s0_radius*cos(a));
    glm::vec2 s1 = glm::vec2(s1_radius*sin(b), s1_radius*cos(b));
    glm::vec2 s2 = glm::vec2(s2_x_radius*sin(c)+s1.x, s2_y_radius*cos(c)+s1.y);

    float uc0_offset = 0.5f * size;
    float uc1_offset = 0.6f * size;
    glm::vec2 uc0 = glm::vec2(d * uc0_offset, uc0_offset);
    glm::vec2 uc1 = glm::vec2(uc1_offset, e);

    glm::vec2 lc0 = glm::vec2(d * uc0_offset, -uc0_offset);
    glm::vec2 lc1 = glm::vec2(uc1_offset, -e);

    Bezier_Curve stem;
    stem.add(glm::vec3(begin, 0.0f));
    stem.add(glm::vec3(s0, 0.0f));
    stem.add(glm::vec3(s1, 0.0f));
    stem.add(glm::vec3(s2, 0.0f));

    Bezier_Curve upper_contour;
    upper_contour.add(glm::vec3(begin, 0.0f));
    upper_contour.add(glm::vec3(uc0, 0.0f));
    upper_contour.add(glm::vec3(uc1, 0.0f));
    upper_contour.add(glm::vec3(s2, 0.0f));

    Bezier_Curve lower_contour;
    lower_contour.add(glm::vec3(begin, 0.0f));
    lower_contour.add(glm::vec3(lc0, 0.0f));
    lower_contour.add(glm::vec3(lc1, 0.0f));
    lower_contour.add(glm::vec3(s2, 0.0f));

    return stem.sample_mesh() + upper_contour.sample_mesh() + lower_contour.sample_mesh();
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

    Mesh leaf = build_leaf();
    leaf.translate_x(-0.5f);
    global.camera.translate(glm::vec3(0.0f, 0.0f, 3.0f));

    while (!global.window.should_close()) {
        global.delta = global.window.update();

        render(global.camera, leaf);

        sleep(10);
    }

    return 0;
}

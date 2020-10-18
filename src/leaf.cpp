#include "leaf.hpp"

#include "bezier_curve.hpp"

#define LEAF_ASPECT (2.0f / 3.0f)

Mesh build_leaf(float f, float g, int v, float s) {
    float x_size = g * s;
    float y_size = x_size * g * LEAF_ASPECT;

    float a = f * (M_PI / v) + (M_PI / 2);
    float b = -a + M_PI;
    float c = a;

    float d_max = 0.5f * x_size;
    float d_min = 0.2f * x_size;
    float d = d_max - g * (d_max-d_min);

    float e_max = 1.0f * y_size;
    float e_min = 0.2f * y_size;
    float e = e_max - g * (e_max-e_min);

    glm::vec2 begin = glm::vec2(0.0f, 0.0f);
    glm::vec2 end = glm::vec2(x_size, 0.0f);

    float s0_radius = 0.4 * s;
    float s1_radius = 0.8 * s;
    float s2_x_radius = 0.2 * s;
    float s2_y_radius = 0.4 * s;
    glm::vec2 s0 = glm::vec2(s0_radius*sin(a), s0_radius*cos(a));
    glm::vec2 s1 = glm::vec2(s1_radius*sin(b), s1_radius*cos(b));
    glm::vec2 s2 = glm::vec2(s2_x_radius*sin(c)+s1.x, s2_y_radius*cos(c)+s1.y);

    float c1_offset = 0.6f * x_size;
    glm::vec2 uc0 = glm::vec2(d, y_size);
    glm::vec2 uc1 = glm::vec2(c1_offset, e);

    glm::vec2 lc0 = glm::vec2(d, -y_size);
    glm::vec2 lc1 = glm::vec2(c1_offset, -e);

    Bezier_Curve stem;
    stem.add(glm::vec3(begin, 0.0f));
    stem.add(glm::vec3(s0, 0.0f));
    stem.add(glm::vec3(s1, 0.0f));
    stem.add(glm::vec3(s2, 0.0f));

    Bezier_Curve upper_contour;
    upper_contour.add(glm::vec3(begin, 0.0f));
    upper_contour.add(glm::vec3(uc0, 0.0f));
    upper_contour.add(glm::vec3(uc1, 0.0f));
    upper_contour.add(glm::vec3(end, 0.0f));

    Bezier_Curve lower_contour;
    lower_contour.add(glm::vec3(begin, 0.0f));
    lower_contour.add(glm::vec3(lc0, 0.0f));
    lower_contour.add(glm::vec3(lc1, 0.0f));
    lower_contour.add(glm::vec3(end, 0.0f));

    /* Mesh stem = stem.sample_mesh_until(-1.0f); */
    Mesh vain = stem.sample_mesh_until(g);
    Mesh margin = upper_contour.sample_mesh() + lower_contour.sample_mesh();

    return vain + margin;
}

Leaf::Leaf(float f, int variance_limiter, float size) : m_f(f), m_size(size), m_variance_limiter(variance_limiter) {
}

Mesh Leaf::mesh(float growth_stage) const {
    return build_leaf(m_f, growth_stage, m_variance_limiter, m_size);
}

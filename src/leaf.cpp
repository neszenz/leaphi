#include "leaf.hpp"

#include "bezier_curve.hpp"

#define LEAF_ASPECT (2.0f / 3.0f)
#define LEAF_VARIANCE 16 // random-generation variance limiter {2...}
#define VEIN_STEM_RATIO (1.0f / 4.0f)

samples_t build_vein_samples(float f, float g, float size_factor) {
    float a = f * (M_PI / LEAF_VARIANCE) + (M_PI / 2);
    float b = -a + M_PI;

    float c0_x = 0.0f;
    float c0_y = 0.0f;

    float c1_radius = 0.4f * size_factor;
    float c1_x = c1_radius * sin(a);
    float c1_y = c1_radius * cos(a);

    float c2_radius = 0.8f * size_factor;
    float c2_x = c2_radius * sin(b);
    float c2_y = c2_radius * cos(b);

    float c3_x_radius = 0.2f * size_factor;
    float c3_y_radius = 2.0f * c3_x_radius;
    float c3_x = c3_x_radius * sin(a) + c2_x;
    float c3_y = c3_y_radius * cos(a) + c2_y;

    Bezier_Curve vein;
    vein.add(glm::vec3(c0_x, c0_y, 0.0f));
    vein.add(glm::vec3(c1_x, c1_y, 0.0f));
    vein.add(glm::vec3(c2_x, c2_y, 0.0f));
    vein.add(glm::vec3(c3_x, c3_y, 0.0f));

    return vein.sample(g);
}
samples_t build_upper_margin_samples(float g, float x_size, float y_size) {
    float c0_x = 0.0f;
    float c0_y = 0.0f;

    float d_max = 0.5f * x_size;
    float d_min = 0.2f * x_size;
    float d = d_max - g * (d_max-d_min);
    float c1_x = d;
    float c1_y = y_size;

    float e_max = 1.0f * y_size;
    float e_min = 0.2f * y_size;
    float e = e_max - g * (e_max-e_min);
    float c2_offset = 0.6f * x_size;
    float c2_x = c2_offset;
    float c2_y = e;

    float c3_x = x_size;
    float c3_y = 0.0f;

    Bezier_Curve upper_margin;
    upper_margin.add(glm::vec3(c0_x, c0_y, 0.0f));
    upper_margin.add(glm::vec3(c1_x, c1_y, 0.0f));
    upper_margin.add(glm::vec3(c2_x, c2_y, 0.0f));
    upper_margin.add(glm::vec3(c3_x, c3_y, 0.0f));

    return upper_margin.sample();
}

void normalize_vein(samples_t& vein, float x_size) {
    float vein_size = vein.back().x;
    for (auto& s : vein) {
        s.x = s.x/vein_size * x_size;
    }
}
void invert_samples_x(samples_t& samples) {
    for (auto& s : samples) {
        s.x = -s.x;
    }
}
void invert_samples_y(samples_t& samples) {
    for (auto& s : samples) {
        s.y = -s.y;
    }
}
void apply_vein_offset(const samples_t& vein, samples_t& margin) {
    assert(vein.size() == margin.size());

    for (unsigned i = 0; i < margin.size(); ++i) {
        margin.at(i).y += vein.at(i).y;
    }
}

Mesh build_leaf(float f, float g, float size_factor) {
    float x_size = g * size_factor;
    float y_size = x_size * g * LEAF_ASPECT;

    samples_t vein_samples = build_vein_samples(f, g, size_factor);
    normalize_vein(vein_samples, x_size);

    unsigned offset = vein_samples.size() * VEIN_STEM_RATIO;
    samples_t stem_samples(vein_samples.begin(), vein_samples.begin()+offset);
    invert_samples_x(stem_samples);
    invert_samples_y(stem_samples);

    samples_t u_margin_samples = build_upper_margin_samples(g, x_size, y_size);
    samples_t l_margin_samples = u_margin_samples;
    invert_samples_y(l_margin_samples);
    apply_vein_offset(vein_samples, u_margin_samples);
    apply_vein_offset(vein_samples, l_margin_samples);

    Mesh vein = Bezier_Curve::mesh_from_samples(vein_samples);
    Mesh stem = Bezier_Curve::mesh_from_samples(stem_samples);
    Mesh u_margin = Bezier_Curve::mesh_from_samples(u_margin_samples);
    Mesh l_margin = Bezier_Curve::mesh_from_samples(l_margin_samples);

    Mesh leaf = stem + vein + u_margin + l_margin;
    leaf.translate(-stem_samples.back());

    return leaf;
}

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Leaf::Leaf(float f, float size_factor) : m_f(f), m_size_factor(size_factor) {
}

Mesh Leaf::mesh(float growth_stage) const {
    return build_leaf(m_f, growth_stage, m_size_factor);
}

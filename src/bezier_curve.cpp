#include "bezier_curve.hpp"

#define CURVE_COLOR glm::vec3(0.0f, 0.0f, 0.0f)
#define CURVE_SHADER Shader("bezier_curve", "res/shaders/basic.vert", \
                                            "res/shaders/basic.frag")

glm::vec3 de_casteljau(const std::vector<glm::vec3>& points, float u) {
    assert(u >= 0.0f && u <= 1.0f);

    int n_points = points.size();
    int n_nodes = (n_points*n_points + n_points) / 2;

    // 0th rank
    std::vector<glm::vec3> nodes(n_nodes - n_points);
    nodes.insert(nodes.begin(), points.begin(), points.end());
    int offset = n_points;

    // remaining ranks
    int n_ranks = n_points-1;
    for (int r = 1; r <= n_ranks; ++r) {
        for (int i = 0; i <= n_ranks-r; ++i) {
            int index = i + offset;
            int r_child = index - (n_points - r);
            int l_child = r_child - 1;
            nodes[index] = (1.0f-u) * nodes[l_child] + u * nodes[r_child];
        }
        offset += n_points - r;
    }

    return nodes.back();
}

std::vector<glm::vec3> sample_bezier_curve(const std::vector<glm::vec3>& points,
                                           unsigned n_samples) {
    n_samples = std::max(2u, n_samples);
    std::vector<glm::vec3> samples;

    for (unsigned i = 0; i < n_samples; ++i) {
        float p = float(i) / (n_samples-1);
        samples.push_back(de_casteljau(points, p));
    }

    return samples;
}

v_buffer_t buffer_from_vector(std::vector<glm::vec3> vector, glm::vec3 color) {
    v_buffer_t buffer;

    for (const glm::vec3& vertex : vector) {
        buffer.push_back(vertex.x);
        buffer.push_back(vertex.y);
        buffer.push_back(vertex.z);
        buffer.push_back(color.r);
        buffer.push_back(color.g);
        buffer.push_back(color.b);
    }

    return buffer;
}

e_buffer_t generate_e_buffer(unsigned n_vertices) {
    e_buffer_t buffer;

    for (unsigned i = 1; i < n_vertices; ++i) {
        buffer.push_back(i-1);
        buffer.push_back(i);
    }

    return buffer;
}

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Bezier_Curve::Bezier_Curve(std::vector<glm::vec3> points) : m_points(std::move(points)) {
}

void Bezier_Curve::add(glm::vec3 point) {
    m_points.push_back(point);
}

Mesh Bezier_Curve::sample_mesh(unsigned n_samples) const {
    std::vector<glm::vec3> samples = sample_bezier_curve(m_points, n_samples);

    v_buffer_t v_buffer = buffer_from_vector(samples, CURVE_COLOR);
    e_buffer_t e_buffer = generate_e_buffer(samples.size());

    return Mesh(Geometry(v_buffer, e_buffer, GL_LINES), CURVE_SHADER);
}

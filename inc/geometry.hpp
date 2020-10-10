#pragma once

#include <vector>

typedef std::vector<float> v_buffer_t;
typedef std::vector<unsigned> e_buffer_t;

class Geometry {
    public:
        Geometry(const v_buffer_t& v_buffer, const e_buffer_t& e_buffer);
        Geometry();

        Geometry(const Geometry& other);
        Geometry& operator=(Geometry other);
        Geometry(Geometry&& other);
        Geometry& operator=(Geometry&& other);
        ~Geometry();

        friend void swap(Geometry& first, Geometry& second);

        void draw() const;
    private:
        struct aabb_t { // axis-aligned bounding box
            float x_min, x_max, y_min, y_max, z_min, z_max;
        } m_bounding_box;

        struct gbi_t { // geometry buffer ids
            unsigned vao, vbo, ebo;
            unsigned long n_elements;
        } m_gbi;

        v_buffer_t m_v_buffer;
        e_buffer_t m_e_buffer;

        aabb_t compute_bounding_box(const v_buffer_t& v_buffer);

        gbi_t create_geometry_buffer(const v_buffer_t& v_buffer,
                                     const e_buffer_t& e_buffer);
};

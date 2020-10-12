#include "util.hpp"

#include <assert.h>
#include <chrono>
#include <sstream>
#include <thread>

std::string to_string(const glm::vec2& value) {
    std::stringstream ss;
    ss << "vec3(" << value.x << ", " << value.y << ")";
    return ss.str();
}
std::string to_string(const glm::vec3& value) {
    std::stringstream ss;
    ss << "vec3(" << value.x << ", " << value.y << ", " << value.z << ")";
    return ss.str();
}

void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

glm::vec3 arc_ball_mapping(glm::vec2 screen_vec, glm::ivec2 screen_size) {
    // Ken Shoemake's ArcBall sphere mapping
    glm::vec2 circle_center(screen_size.x/2, screen_size.y/2);
    double radius = screen_size.y/2;

    double x =  (screen_vec.x - circle_center.x) / radius;
    double y = -(screen_vec.y - circle_center.y) / radius;
    double z = 0.0;
    double r = x*x + y*y;

    if (1.0 - r <= 0.0) { // modified condition fixes rounding issue for sqrt
        assert(r > 1.0);
        double s = 1.0 / std::sqrt(r);
        x = s * x;
        y = s * y;
    } else {
        assert(1.0 - r > 0.0); // should be redundant due to modified condition
        z = std::sqrt(1.0 - r);
    }

    return glm::vec3(x, y, z);
}

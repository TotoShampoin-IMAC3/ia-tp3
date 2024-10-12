#pragma once
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

inline glm::mat4 hyperbolicTranslationMatrix(const glm::vec3& translation) {
    auto c = glm::cosh(translation);
    auto s = glm::sinh(translation);

    return glm::mat4 {c.x, 0, 0, s.x, 0, 0, 1, 0, 0, 1, 0, 0, s.x, 0, 0, c.x} *
           glm::mat4 {1, 0, 0, 0, 0, c.y, 0, s.y, 0, 0, 1, 0, 0, s.y, 0, c.y} *
           glm::mat4 {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, c.z, s.z, 0, 0, s.z, c.z};
}

inline float tileSizeEucl(int squares_at_a_vertex) {
    float angle = glm::pi<float>() / squares_at_a_vertex;
    float leg = glm::pow(glm::cos(angle), 2) - 0.5;
    return glm::sqrt(leg) / glm::cos(angle);
}
inline float tileDistance(int squares_at_a_vertex) {
    float angle = glm::pi<float>() / squares_at_a_vertex;
    float leg = glm::cos(angle) / glm::sqrt(.5);
    return 2 * glm::acosh(leg);
}

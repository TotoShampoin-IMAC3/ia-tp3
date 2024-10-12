#pragma once
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

/**
 * @brief The size of a square tile in H3 hyperbolic space.
 *
 */
inline float tileSizeEucl(int squares_at_a_vertex) {
    float angle = glm::pi<float>() / squares_at_a_vertex;
    float leg = glm::pow(glm::cos(angle), 2) - 0.5;
    return glm::sqrt(leg) / glm::cos(angle);
}

/**
 * @brief The distance between the centers of two adjacent tiles in H3 hyperbolic space.
 *
 */
inline float tileDistance(int squares_at_a_vertex) {
    float angle = glm::pi<float>() / squares_at_a_vertex;
    float leg = glm::cos(angle) / glm::sqrt(.5);
    return 2 * glm::acosh(leg);
}

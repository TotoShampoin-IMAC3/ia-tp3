#pragma once
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

inline glm::mat4 hyperbolicTranslationMatrix(const glm::vec3& translation) {
    auto c = glm::cosh(translation);
    auto s = glm::sinh(translation);

    return glm::mat4 {c.x, 0, 0, s.x, 0, 0, 1, 0, 0, 1, 0, 0, s.x, 0, 0, c.x} *
           glm::mat4 {1, 0, 0, 0, 0, c.y, 0, s.y, 0, 0, 1, 0, 0, s.y, 0, c.y} *
           glm::mat4 {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, c.z, s.z, 0, 0, s.z, c.z};
}
#include "HyperTransform.hpp"
#include <glm/ext/matrix_transform.hpp>

glm::mat4 HyperTransform::translation(const glm::vec3& translation) {
    auto c = glm::cosh(translation);
    auto s = glm::sinh(translation);

    return glm::mat4 {c.x, 0, 0, s.x, 0, 1, 0, 0, 0, 0, 1, 0, s.x, 0, 0, c.x} *
           glm::mat4 {1, 0, 0, 0, 0, c.y, 0, s.y, 0, 0, 1, 0, 0, s.y, 0, c.y} *
           glm::mat4 {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, c.z, s.z, 0, 0, s.z, c.z};
}
glm::mat4 HyperTransform::rotation(const glm::vec3& axis, float angle) {
    return glm::rotate(glm::mat4(1), angle, axis);
}

HyperTransform HyperTransform::translated(const glm::vec3& translation) const {
    HyperTransform transform = *this;
    transform.translate(translation);
    return transform;
}

HyperTransform HyperTransform::rotated(const glm::vec3& axis, float angle) const {
    HyperTransform transform = *this;
    transform.rotate(axis, angle);
    return transform;
}

void HyperTransform::translate(const glm::vec3& translation) {
    _matrix = _matrix * HyperTransform::translation(translation);
}

void HyperTransform::rotate(const glm::vec3& axis, float angle) {
    _matrix = _matrix * rotation(axis, angle);
}

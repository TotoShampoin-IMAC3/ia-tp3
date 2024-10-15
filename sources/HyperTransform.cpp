#include "HyperTransform.hpp"
#include <glm/ext/matrix_transform.hpp>

glm::dmat4 HyperTransform::translation(const glm::dvec3& translation) {
    auto c = glm::cosh(translation);
    auto s = glm::sinh(translation);

    return glm::dmat4 {c.x, 0, 0, s.x, 0, 1, 0, 0, 0, 0, 1, 0, s.x, 0, 0, c.x} *
           glm::dmat4 {1, 0, 0, 0, 0, c.y, 0, s.y, 0, 0, 1, 0, 0, s.y, 0, c.y} *
           glm::dmat4 {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, c.z, s.z, 0, 0, s.z, c.z};
}
glm::dmat4 HyperTransform::rotation(const glm::dvec3& axis, double angle) {
    return glm::rotate(glm::dmat4(1), angle, axis);
}

HyperTransform HyperTransform::translated(const glm::dvec3& translation) const {
    HyperTransform transform = *this;
    transform.translate(translation);
    return transform;
}

HyperTransform HyperTransform::rotated(const glm::dvec3& axis, double angle) const {
    HyperTransform transform = *this;
    transform.rotate(axis, angle);
    return transform;
}

void HyperTransform::translate(const glm::dvec3& translation) {
    _matrix = _matrix * HyperTransform::translation(translation);
}

void HyperTransform::rotate(const glm::dvec3& axis, double angle) {
    _matrix = _matrix * rotation(axis, angle);
}

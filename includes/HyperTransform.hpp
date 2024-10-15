#pragma once

#include <glm/glm.hpp>

/**
 * @brief A class to represent a H3 hyperbolic transformation.
 *
 */
class HyperTransform {
public:
    static glm::dmat4 translation(const glm::dvec3& translation);
    static glm::dmat4 rotation(const glm::dvec3& axis, double angle);

    HyperTransform translated(const glm::dvec3& translation) const;
    HyperTransform rotated(const glm::dvec3& axis, double angle) const;

    void translate(const glm::dvec3& translation);
    void rotate(const glm::dvec3& axis, double angle);

    glm::dmat4 matrix() const { return _matrix; }
    glm::dmat4& matrix() { return _matrix; }

    operator glm::dmat4() const { return _matrix; }

private:
    glm::dmat4 _matrix {1.0f};
};

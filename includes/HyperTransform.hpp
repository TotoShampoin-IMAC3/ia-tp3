#pragma once

#include <glm/glm.hpp>

/**
 * @brief A class to represent a H3 hyperbolic transformation.
 *
 */
class HyperTransform {
public:
    static glm::mat4 translation(const glm::vec3& translation);
    static glm::mat4 rotation(const glm::vec3& axis, float angle);

    HyperTransform translated(const glm::vec3& translation) const;
    HyperTransform rotated(const glm::vec3& axis, float angle) const;

    void translate(const glm::vec3& translation);
    void rotate(const glm::vec3& axis, float angle);

    glm::mat4 matrix() const { return _matrix; }
    glm::mat4& matrix() { return _matrix; }

    operator glm::mat4() const { return _matrix; }

private:
    glm::mat4 _matrix {1.0f};
};

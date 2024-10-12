#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

class FPSController {
public:
    void rotate(const glm::vec2& mouse_delta) {
        _pitch += mouse_delta.y * 0.01f;
        _yaw += mouse_delta.x * 0.01f;
    }

    float& pitch() { return _pitch; }
    float& yaw() { return _yaw; }

    float pitch() const { return _pitch; }
    float yaw() const { return _yaw; }

    glm::mat4 pitchMatrix() const { return glm::rotate(glm::mat4(1), _pitch, glm::vec3(1, 0, 0)); }
    glm::mat4 yawMatrix() const { return glm::rotate(glm::mat4(1), _yaw, glm::vec3(0, 0, 1)); }

private:
    float _pitch = 0.0f;
    float _yaw = 0.0f;
};

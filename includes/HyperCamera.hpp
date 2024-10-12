#pragma once

#include "HyperTransform.hpp"

enum class HyperbolicProjection {
    BeltramiKlein,
    PoincareDisk,
    GansModel
};

class HyperCamera {
public:
    HyperCamera() = default;
    HyperCamera(const HyperbolicProjection& projection)
        : _projection(projection) {}

    void setProjectionMode(const HyperbolicProjection& projection);

    HyperTransform& transform() { return _transform; }
    const HyperTransform& transform() const { return _transform; }

    HyperbolicProjection& projection() { return _projection; }
    HyperbolicProjection projection() const { return _projection; }

    glm::vec3& eyeOffset() { return _eye_offset; }
    const glm::vec3& eyeOffset() const { return _eye_offset; }

    glm::mat4 viewMatrix() const { return glm::inverse(_transform.translated(_eye_offset).matrix()); }

private:
    HyperTransform _transform;
    HyperbolicProjection _projection;
    glm::vec3 _eye_offset = glm::vec3(0.0f, 0.0f, 0.0f);
};

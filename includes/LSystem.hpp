#pragma once

#include "HyperMesh.hpp"
#include <string>

class LSystemRule {
public:
    LSystemRule() = default;
    LSystemRule(const std::string& path, float angle = 20.0f, float length = 0.1f)
        : _path(path),
          _angle(angle),
          _length(length) {}

    HyperModel generateHyperbolic();
    HyperModel generateHyperbolic(int N);

    std::string path() const { return _path; }
    float angle() const { return _angle; }
    float length() const { return _length; }

    std::string& path() { return _path; }
    float& angle() { return _angle; }
    float& length() { return _length; }

private:
    std::string _path;
    float _angle;
    float _length;
};

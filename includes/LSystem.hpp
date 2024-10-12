#pragma once

#include "HyperMesh.hpp"
#include <string>

class LSystemPath {
public:
    LSystemPath() = default;
    LSystemPath(const std::string& path, float angle = 20.0f, float length = 0.1f)
        : _path(path),
          _angle(angle),
          _length(length) {}

    HyperMesh generateHyperbolic();

    std::string path() const { return _path; }
    float angle() const { return _angle; }
    float length() const { return _length; }

    std::string& path() { return _path; }
    float angle() { return _angle; }
    float length() { return _length; }

private:
    std::string _path;
    float _angle;
    float _length;
};

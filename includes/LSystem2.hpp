#pragma once

#include "HyperMesh.hpp"
#include <map>
#include <string>
#include <vector>

struct LSystem {
    LSystem() = default;
    LSystem(const std::string& axiom, const std::string& rules, float angle)
        : axiom(axiom),
          angle(angle) {
        setRules(axiom, rules);
    }

    HyperModel generate(const float& length, const int& N);

    void setRules(const std::string& axiom, const std::string& rules);

    std::string axiom;
    std::map<char, std::string> rules;
    std::vector<char> nodraw;
    float angle;

private:
    struct Path {
        std::vector<glm::dvec4> vertices;
        std::vector<unsigned int> indices;
    };

    Path generatePath(const float& length, const int& N);

    void iterate(char c, const int& N, std::string& output);
};

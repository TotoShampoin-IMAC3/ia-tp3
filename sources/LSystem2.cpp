#include "LSystem2.hpp"
#include "HyperMesh.hpp"
#include "HyperTransform.hpp"
#include "misc.hpp"
#include <stack>

void LSystem::setRules(const std::string& axiom, const std::string& rules) {
    this->axiom = axiom;
    this->rules.clear();
    for (const auto& rule_str : split(rules, "\n")) {
        if (rule_str.size() < 3) {
            continue;
        }
        if (rule_str[0] == '#') {
            continue;
        }
        if (rule_str[1] != '=') {
            continue;
        }
        this->rules[rule_str[0]] = rule_str.substr(2);
    }
}

HyperModel LSystem::generate(const float& length, const int& N) {
    Path model = generatePath(length, N);
    HyperModel hyper_model;
    hyper_model.vertices.reserve(model.vertices.size());
    for (const auto& vertex : model.vertices) {
        hyper_model.vertices.push_back({glm::vec4(vertex), glm::vec2(0)});
    }
    hyper_model.indices = model.indices;
    return hyper_model;
}

LSystem::Path LSystem::generatePath(const float& length, const int& N) {
    Path model;
    std::string path;
    for (const auto& c : axiom) {
        iterate(c, N, path);
    }
    HyperTransform turtle;
    int current_vertex = 0;
    std::stack<std::tuple<HyperTransform, int>> stack;

    auto placeVertex = [&]() {
        auto vertex = turtle.matrix() * glm::dvec4(0, 0, 0, 1);
        model.vertices.push_back(vertex);
        model.indices.push_back(current_vertex);
        model.indices.push_back(model.vertices.size() - 1);
        current_vertex = model.vertices.size() - 1;
    };

    model.vertices.push_back(glm::dvec4(0, 0, 0, 1));
    for (const auto& c : path) {
        switch (c) {
        default: {
            turtle.translate(glm::dvec3(0, 0, length));
            placeVertex();
        } break;
        case '+': turtle.rotate(glm::dvec3(1, 0, 0), glm::radians(angle)); break;
        case '-': turtle.rotate(glm::dvec3(1, 0, 0), glm::radians(-angle)); break;
        case '&': turtle.rotate(glm::dvec3(0, 1, 0), glm::radians(angle)); break;
        case '^': turtle.rotate(glm::dvec3(0, 1, 0), glm::radians(-angle)); break;
        case '<': turtle.rotate(glm::dvec3(0, 0, 1), glm::radians(angle)); break;
        case '>': turtle.rotate(glm::dvec3(0, 0, 1), glm::radians(-angle)); break;
        case '|': turtle.rotate(glm::dvec3(0, 0, 1), glm::radians(180.0f)); break;
        case '[': stack.push({turtle, current_vertex}); break;
        case ']': {
            const auto [transform, vertex] = stack.top();
            turtle = transform;
            current_vertex = vertex;
            stack.pop();
        } break;
        }
    }
    return model;
}

void LSystem::iterate(char c, const int& N, std::string& output) {
    if (N == 0) {
        output.push_back(c);
        return;
    }
    if (rules.find(c) == rules.end()) {
        output.push_back(c);
        return;
    }
    for (const auto& rule_c : rules[c]) {
        iterate(rule_c, N - 1, output);
    }
}
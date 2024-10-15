#include "LSystem.hpp"
#include "HyperMesh.hpp"
#include "HyperTransform.hpp"
#include <glm/fwd.hpp>
#include <stack>
#include <tuple>
#include <vector>

HyperModel LSystemRule::generateHyperbolic() {
    // std::vector<HyperVertex> vertices;
    std::vector<glm::dvec4> vertices;
    std::vector<unsigned int> indices;

    HyperTransform turtle;
    int current_vertex = 0;
    std::stack<std::tuple<HyperTransform, int>> stack;

    auto placeVertex = [&]() {
        auto vertex = turtle.matrix() * glm::dvec4(0, 0, 0, 1);
        vertices.push_back(vertex);
        indices.push_back(current_vertex);
        indices.push_back(vertices.size() - 1);
        current_vertex = vertices.size() - 1;
    };

    vertices.push_back(glm::dvec4(0, 0, 0, 1));
    for (auto c : _path) {
        switch (c) {
        case 'F': {
            turtle.translate(glm::dvec3(0, 0, _length));
            placeVertex();
        } break;
        case '+': turtle.rotate(glm::dvec3(1, 0, 0), glm::radians(_angle)); break;
        case '-': turtle.rotate(glm::dvec3(1, 0, 0), glm::radians(-_angle)); break;
        case '&': turtle.rotate(glm::dvec3(0, 1, 0), glm::radians(_angle)); break;
        case '^': turtle.rotate(glm::dvec3(0, 1, 0), glm::radians(-_angle)); break;
        case '<': turtle.rotate(glm::dvec3(0, 0, 1), glm::radians(_angle)); break;
        case '>': turtle.rotate(glm::dvec3(0, 0, 1), glm::radians(-_angle)); break;
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

    std::vector<HyperVertex> hyper_vertices;
    for (auto& vertex : vertices) {
        hyper_vertices.push_back({glm::vec4(vertex), glm::vec2(0)});
    }

    return HyperModel(hyper_vertices, indices);
}

HyperModel LSystemRule::generateHyperbolic(int N) {
    std::string path = _path;
    for (int i = 0; i < N; i++) {
        std::string new_path;
        for (auto c : path) {
            if (c == 0)
                break;
            switch (c) {
            case 'F': new_path += _path; break;
            default: new_path += c;
            }
        }
        path = new_path;
    }
    return LSystemRule(path, _angle, _length).generateHyperbolic();
}

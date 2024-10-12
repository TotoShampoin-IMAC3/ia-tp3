#include "LSystem.hpp"
#include "HyperMesh.hpp"
#include "HyperTransform.hpp"
#include <stack>
#include <tuple>
#include <vector>

HyperMesh LSystemPath::generateHyperbolic() {
    std::vector<HyperVertex> vertices;
    std::vector<unsigned int> indices;

    HyperTransform turtle;
    int current_vertex = 0;
    std::stack<std::tuple<HyperTransform, int>> stack;

    auto placeVertex = [&]() {
        auto vertex = HyperVertex {turtle.matrix() * glm::vec4(0, 0, 0, 1), glm::vec2(0, 0)};
        vertices.push_back(vertex);
        indices.push_back(current_vertex);
        indices.push_back(vertices.size() - 1);
        current_vertex = vertices.size() - 1;
    };

    vertices.push_back(HyperVertex {glm::vec4(0, 0, 0, 1), glm::vec2(0, 0)});
    for (auto c : _path) {
        switch (c) {
        case 'F': {
            turtle.translate(glm::vec3(0, 0, _length));
            placeVertex();
        } break;
        case '+': turtle.rotate(glm::vec3(1, 0, 0), glm::radians(_angle)); break;
        case '-': turtle.rotate(glm::vec3(1, 0, 0), glm::radians(-_angle)); break;
        case '&': turtle.rotate(glm::vec3(0, 1, 0), glm::radians(_angle)); break;
        case '^': turtle.rotate(glm::vec3(0, 1, 0), glm::radians(-_angle)); break;
        case '<': turtle.rotate(glm::vec3(0, 0, 1), glm::radians(_angle)); break;
        case '>': turtle.rotate(glm::vec3(0, 0, 1), glm::radians(-_angle)); break;
        case '|': turtle.rotate(glm::vec3(0, 0, 1), glm::radians(180.0f)); break;
        case '[': stack.push({turtle, current_vertex}); break;
        case ']': {
            const auto [transform, vertex] = stack.top();
            turtle = transform;
            current_vertex = vertex;
            stack.pop();
        } break;
        }
    }

    return HyperMesh(vertices, indices, GL_LINES);
}

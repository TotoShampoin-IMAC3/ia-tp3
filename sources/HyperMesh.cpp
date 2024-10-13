#include "HyperMesh.hpp"
#include <glm/ext/scalar_constants.hpp>

HyperMesh::HyperMesh(const std::vector<HyperVertex>& vertices, const std::vector<GLuint>& indices, GLenum mode)
    : mode(mode) {
    vbo.bind();
    vbo.data(vertices, GL_STATIC_DRAW);
    vao.bind();
    vao.setAttribPointer<HyperVertex, glm::vec4, offsetof(HyperVertex, position)>(0);
    vao.setAttribPointer<HyperVertex, glm::vec2, offsetof(HyperVertex, texcoord)>(1);
    ibo.bind();
    ibo.data(indices, GL_STATIC_DRAW);
    index_count = indices.size();
}

void HyperMesh::draw() const {
    vao.bind();
    glDrawElements(mode, index_count, GL_UNSIGNED_INT, nullptr);
}

void HyperMesh::set(const std::vector<HyperVertex>& vertices, const std::vector<GLuint>& indices) {
    vbo.data(vertices, GL_STATIC_DRAW);
    ibo.data(indices, GL_STATIC_DRAW);
    index_count = indices.size();
}

float tileSizeEucl(int squares_at_a_vertex) {
    float angle = glm::pi<float>() / squares_at_a_vertex;
    float leg = glm::pow(glm::cos(angle), 2) - 0.5;
    return glm::sqrt(leg) / glm::cos(angle);
}
float tileDistance(int squares_at_a_vertex) {
    float angle = glm::pi<float>() / squares_at_a_vertex;
    float leg = glm::cos(angle) / glm::sqrt(.5);
    return 2 * glm::acosh(leg);
}

std::vector<HyperVertex> hyperbolize(const std::vector<toto::Vertex>& vertices, const int& squares_at_a_vertex) {
    std::vector<HyperVertex> hyperbolizedVertices;
    auto tileSize = tileSizeEucl(squares_at_a_vertex);
    auto tileDist = tileDistance(squares_at_a_vertex);
    for (const auto& vert : vertices) {
        glm::vec4 position = glm::vec4(vert.position, 1.0f);
        auto d = glm::sqrt(1 - glm::dot(vert.position, vert.position));
        if (d == 0)
            d = 0.0001f;
        hyperbolizedVertices.push_back(HyperVertex {position / d, vert.texcoord});
    }
    return hyperbolizedVertices;
}

std::vector<HyperVertex> hyperbolize(const std::vector<HyperVertex>& vertices, const int& squares_at_a_vertex) {
    std::vector<HyperVertex> hyperbolizedVertices;
    auto tileSize = tileSizeEucl(squares_at_a_vertex);
    auto tileDist = tileDistance(squares_at_a_vertex);
    for (const auto& vert : vertices) {
        glm::vec4 position = vert.position;
        auto d = glm::sqrt(1 - glm::dot(glm::vec3(vert.position), glm::vec3(vert.position)));
        if (d == 0)
            d = 0.0001f;
        hyperbolizedVertices.push_back(HyperVertex {position / d, vert.texcoord});
    }
    return hyperbolizedVertices;
}

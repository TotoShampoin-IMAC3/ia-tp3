#pragma once

#include "toto-engine/gl/globjects.hpp"
#include "toto-engine/mesh.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct HyperVertex {
    glm::vec4 position;
    glm::vec2 texcoord;
};

struct HyperModel {
    std::vector<HyperVertex> vertices;
    std::vector<GLuint> indices;
};

struct HyperMesh {
    toto::GLBuffer<toto::GLBufferTarget::Array> vbo;
    toto::GLVertexArray<> vao;
    toto::GLBuffer<toto::GLBufferTarget::ElementArray> ibo;
    GLsizei index_count;
    GLenum mode;

    HyperMesh(const std::vector<HyperVertex>& vertices, const std::vector<GLuint>& indices, GLenum mode);
    void draw() const;
};

std::vector<HyperVertex> hyperbolize(const std::vector<toto::Vertex>& vertices, const int& squares_at_a_vertex);
std::vector<HyperVertex> hyperbolize(const std::vector<HyperVertex>& vertices, const int& squares_at_a_vertex);

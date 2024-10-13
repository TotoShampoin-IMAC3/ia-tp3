#pragma once

#include "toto-engine/gl/globjects.hpp"
#include "toto-engine/mesh.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

/**
 * @brief A vertex destined to render in H3 hyperbolic space.
 *
 * Why don't I use the regular Vertex of TotoEngine?
 * Because H3 hyperbolic space exists within a 4D hyperboloide, meaning I need a 4D vector.
 *
 */
struct HyperVertex {
    glm::vec4 position;
    glm::vec2 texcoord;
};

/**
 * @brief A 3D model stored in CPU for H3 hyperbolic space.
 * I plan to add the same thing for euclidean space in the TotoEngine.
 */
struct HyperModel {
    std::vector<HyperVertex> vertices;
    std::vector<GLuint> indices;
};

/**
 * @brief Similar to toto::Mesh, but for H3 hyperbolic space.
 * There is also a mode component, which is the same as the mode in glDrawElements. I plan to add that to toto::Mesh.
 */
struct HyperMesh {
    toto::GLBuffer<toto::GLBufferTarget::Array> vbo;
    toto::GLVertexArray<> vao;
    toto::GLBuffer<toto::GLBufferTarget::ElementArray> ibo;
    GLsizei index_count;
    GLenum mode;

    HyperMesh(const std::vector<HyperVertex>& vertices, const std::vector<GLuint>& indices, GLenum mode);
    void draw() const;

    void set(const std::vector<HyperVertex>& vertices, const std::vector<GLuint>& indices);
};

/**
 * @brief Project a mesh from euclidean space to H3 hyperbolic space through the Beltrami-Klein model.
 *
 */
std::vector<HyperVertex> hyperbolize(const std::vector<toto::Vertex>& vertices, const int& squares_at_a_vertex);
std::vector<HyperVertex> hyperbolize(const std::vector<HyperVertex>& vertices, const int& squares_at_a_vertex);

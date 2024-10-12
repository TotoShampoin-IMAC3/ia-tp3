#include "HyperRenderer.hpp"

HyperRenderer::HyperRenderer() {
    _program.use();
    _uniforms["u_projection_euclidean"] = toto::Uniform(_program, "u_projection_euclidean");
    _uniforms["u_view_euclidean"] = toto::Uniform(_program, "u_view_euclidean");
    _uniforms["u_projection_mode"] = toto::Uniform(_program, "u_projection_mode");
    _uniforms["u_view"] = toto::Uniform(_program, "u_view");
    _uniforms["u_model"] = toto::Uniform(_program, "u_model");
    _uniforms["u_color"] = toto::Uniform(_program, "u_color");
    _uniforms["u_texture"] = toto::Uniform(_program, "u_texture");
    _uniforms["u_use_texture"] = toto::Uniform(_program, "u_use_texture");
}

void HyperRenderer::useProgram() {
    _program.use();
}
void HyperRenderer::enableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}
void HyperRenderer::disableDepthTest() {
    glDisable(GL_DEPTH_TEST);
}
void HyperRenderer::clear(bool color, bool depth) {
    glClear((color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0));
}
void HyperRenderer::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void HyperRenderer::setEuclideanProjectionMatrix(const glm::mat4& projection_matrix) {
    _uniforms["u_projection_euclidean"].set(projection_matrix);
}
void HyperRenderer::setEuclideanViewMatrix(const glm::mat4& view_matrix) {
    _uniforms["u_view_euclidean"].set(view_matrix);
}
void HyperRenderer::setHyperbolicProjection(HyperbolicProjection projection) {
    _uniforms["u_projection_mode"].set(static_cast<int>(projection));
}
void HyperRenderer::setHyperbolicViewMatrix(const glm::mat4& view_matrix) {
    _uniforms["u_view"].set(view_matrix);
}
void HyperRenderer::setHyperbolicModelMatrix(const glm::mat4& model_matrix) {
    _uniforms["u_model"].set(model_matrix);
}

void HyperRenderer::setColor(const glm::vec3& color) {
    _uniforms["u_color"].set(color);
}
void HyperRenderer::setTexture(const std::optional<std::reference_wrapper<toto::GLTexture2D>>& texture) {
    _uniforms["u_use_texture"].set(texture.has_value());
    if (texture.has_value()) {
        texture->get().bind();
        _uniforms["u_texture"].set(texture->get(), 0);
    }
}

void HyperRenderer::setTransform(const HyperTransform& transform) {
    setHyperbolicModelMatrix(transform.matrix());
}
void HyperRenderer::setCamera(const HyperCamera& camera) {
    setHyperbolicProjection(camera.projection());
    setHyperbolicViewMatrix(camera.viewMatrix());
}
void HyperRenderer::setEuclideanCamera(const toto::Camera& camera) {
    setEuclideanProjectionMatrix(camera.projectionMatrix());
    setEuclideanViewMatrix(camera.viewMatrix());
}

void HyperRenderer::render(const HyperMesh& model) {
    model.draw();
}
void HyperRenderer::render(const HyperMesh& model, const HyperTransform& transform) {
    setTransform(transform);
    render(model);
}

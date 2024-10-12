#pragma once

#include "HyperCamera.hpp"
#include "HyperMesh.hpp"
#include "HyperTransform.hpp"
#include "toto-engine/uniform.hpp"
#include "toto-engine/utils/camera.hpp"
#include <toto-engine/loader/shader.hpp>
#include <unordered_map>

class HyperRenderer {
public:
    HyperRenderer();

    void useProgram();
    void enableDepthTest();
    void disableDepthTest();
    void clear(bool color = true, bool depth = true);
    void setViewport(int x, int y, int width, int height);

    void setEuclideanProjectionMatrix(const glm::mat4& projection_matrix);
    void setEuclideanViewMatrix(const glm::mat4& view_matrix);
    void setHyperbolicProjection(HyperbolicProjection projection);
    void setHyperbolicViewMatrix(const glm::mat4& view_matrix);
    void setHyperbolicModelMatrix(const glm::mat4& model_matrix);

    void setColor(const glm::vec3& color);
    void setTexture(const std::optional<std::reference_wrapper<toto::GLTexture2D>>& texture);

    void setTransform(const HyperTransform& transform);
    void setCamera(const HyperCamera& camera);
    void setEuclideanCamera(const toto::Camera& camera);

    void render(const HyperMesh& model);
    void render(const HyperMesh& model, const glm::vec3& color, const HyperTransform& transform);

private:
    toto::GLProgram _program = toto::loadRenderShaderFile("resources/hyper.vert", "resources/hyper.frag");
    std::unordered_map<std::string, toto::Uniform> _uniforms;
};

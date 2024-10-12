#include "FPSController.hpp"
#include "HyperCamera.hpp"
#include "HyperMesh.hpp"
#include "HyperRenderer.hpp"
#include "HyperTransform.hpp"
#include "data.hpp"
#include "misc.hpp"
#include "shapes.hpp"
#include "toto-engine/utils/camera.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <random>
#include <toto-engine/gl/glresources.hpp>
#include <toto-engine/mesh.hpp>
#include <toto-engine/uniform.hpp>
#include <toto-engine/window.hpp>
#include <vector>

// The matrices needed for hyperbolic rendering are generated in the CPU
// The GPU only needs to multiply them.

// There are two parts for the rendering:
// - The projection from hyperbolic space to euclidean space
//   -> u_projection_euclidean and u_view_euclidean
// - The projection from euclidean space to screen space
//   -> u_projection_mode, u_view and u_model

// The projection mode determines how hyperbolic space is projected to euclidean space.
// - Beltrami-Klein is better for "immersion mode", as it maintains linearity.
// - Pointcaré Disk is the most visually appealing, it maintains angles and shows how curved the space is.
// - The Gans model is the most stretchy one, as it is simply a flat projection.
// From the center, All three look identical.

// The euclidean camera can be anywhere to visualize the hyperbolic thing
// Placed at the origin, it serves to immerse the user in the hyperbolic space
// Placed outside, it allows the user to have a better idea of the hyperbolic curvature

int main(int argc, const char* argv[]) {
    int width = 1440;
    int height = 900;
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    int squares_at_a_vertex = 5;
    float speed = 0.5f;

    auto window = toto::Window(width, height, "Hyperbolic");
    window.makeContextCurrent();
    toto::Window::initGL();

    const float size = tileSizeEucl(squares_at_a_vertex) * 2.;
    const float distance = tileDistance(squares_at_a_vertex);

    auto hyper_plane = plane(size, size, squares_at_a_vertex);
    hyper_plane.vertices = hyperbolize(hyper_plane.vertices, squares_at_a_vertex);
    auto hyper_mesh = HyperMesh(hyper_plane.vertices, hyper_plane.indices, GL_TRIANGLES);

    auto renderer = HyperRenderer();
    auto euclidean_camera = toto::Camera::Perspective(glm::radians(70.0f), aspect, 0.01f, 100.0f);
    auto camera = HyperCamera(HyperbolicProjection::BeltramiKlein);
    // auto camera = HyperCamera(HyperbolicProjection::PoincareDisk);

    euclidean_camera.transform().position() = glm::vec3(0.0f, 0.0f, 0.0f);
    euclidean_camera.transform().lookAt(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // euclidean_camera.transform().position() = glm::vec3(0, 0, 1.5);
    // euclidean_camera.transform().lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    camera.eyeOffset() = glm::vec3(0.0f, 0.0f, .15f);

    renderer.useProgram();

    renderer.setCamera(camera);
    renderer.setEuclideanCamera(euclidean_camera);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.25f, .75f);

    // A simple hyperbolic tiling
    // In hyperbolic space, up-left is not the same as left-up
    auto grid = {
        HyperTransform(),
        HyperTransform().translated(glm::vec3(1, 0, 0) * distance),
        HyperTransform().translated(glm::vec3(-1, 0, 0) * distance),
        HyperTransform().translated(glm::vec3(0, 1, 0) * distance),
        HyperTransform().translated(glm::vec3(0, -1, 0) * distance),
        HyperTransform().translated(glm::vec3(1, 0, 0) * distance).translated(glm::vec3(1, 0, 0) * distance),
        HyperTransform().translated(glm::vec3(1, 0, 0) * distance).translated(glm::vec3(0, 1, 0) * distance),
        HyperTransform().translated(glm::vec3(1, 0, 0) * distance).translated(glm::vec3(0, -1, 0) * distance),
        HyperTransform().translated(glm::vec3(-1, 0, 0) * distance).translated(glm::vec3(-1, 0, 0) * distance),
        HyperTransform().translated(glm::vec3(-1, 0, 0) * distance).translated(glm::vec3(0, 1, 0) * distance),
        HyperTransform().translated(glm::vec3(-1, 0, 0) * distance).translated(glm::vec3(0, -1, 0) * distance),
        HyperTransform().translated(glm::vec3(0, 1, 0) * distance).translated(glm::vec3(0, 1, 0) * distance),
        HyperTransform().translated(glm::vec3(0, 1, 0) * distance).translated(glm::vec3(1, 0, 0) * distance),
        HyperTransform().translated(glm::vec3(0, 1, 0) * distance).translated(glm::vec3(-1, 0, 0) * distance),
        HyperTransform().translated(glm::vec3(0, -1, 0) * distance).translated(glm::vec3(0, -1, 0) * distance),
        HyperTransform().translated(glm::vec3(0, -1, 0) * distance).translated(glm::vec3(1, 0, 0) * distance),
        HyperTransform().translated(glm::vec3(0, -1, 0) * distance).translated(glm::vec3(-1, 0, 0) * distance),
    };
    std::vector<glm::vec3> colors;
    for (int i = 0; i < grid.size(); i++) {
        colors.push_back(glm::vec3(dis(gen), dis(gen), dis(gen)));
    }

    FPSController controller;

    glm::vec3 velocity(0.0f);
    bool locked = false;

    auto callback_data = CallbackData {velocity, locked};
    handleCallbacks(window, callback_data);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_NONE);

    auto start = glfwGetTime();
    auto last = start;
    while (!window.shouldClose()) {
        auto now = glfwGetTime();
        auto time = now - start;
        auto delta = now - last;

        if (locked) {
            glfwSetInputMode(window.handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            controller.rotate(callback_data.mouse_delta);
        } else {
            glfwSetInputMode(window.handle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        euclidean_camera.transform().rotation() = glm::vec3(0.0f, 0.0f, 0.0f);
        euclidean_camera.transform().rotate(glm::vec3(0, 0, 1), controller.yaw());
        euclidean_camera.transform().rotate(glm::vec3(1, 0, 0), controller.pitch());
        auto rotated_velocity = glm::vec3(controller.yawMatrix() * glm::vec4(velocity, 1.0f));
        camera.transform().translate(float(delta) * rotated_velocity * speed);

        renderer.setEuclideanCamera(euclidean_camera);
        renderer.setCamera(camera);

        renderer.clear();
        int i = 0;
        for (auto& transform : grid) {
            auto color = colors[i % colors.size()];
            renderer.render(hyper_mesh, color, transform);
            i++;
        }

        callback_data.updateDeltas();
        window.pollEvents();
        window.swapBuffers();
        last = now;
    }

    return 0;
}
#include "FPSController.hpp"
#include "HyperCamera.hpp"
#include "HyperMesh.hpp"
#include "HyperRenderer.hpp"
#include "HyperTransform.hpp"
#include "LSystem.hpp"
#include "data.hpp"
#include "misc.hpp"
#include "shapes.hpp"
#include "toto-engine/import-gl.hpp"
#include "toto-engine/loader/image.hpp"
#include "toto-engine/utils/camera.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/trigonometric.hpp>
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

    auto path = LSystemPath("FF>+[+F-F-F]-[-F+F+F]", 22.5f, .05f);
    auto hyper_tree = path.generateHyperbolic(4);

    auto hyper_plane = plane(size, size, squares_at_a_vertex);
    hyper_plane.vertices = hyperbolize(hyper_plane.vertices, squares_at_a_vertex);
    auto hyper_mesh = HyperMesh(hyper_plane.vertices, hyper_plane.indices, GL_TRIANGLES);

    auto grass = toto::loadTexture2D("resources/grass.jpg");

    auto renderer = HyperRenderer();
    auto euclidean_camera = toto::Camera::Perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);

    euclidean_camera.transform().position() = glm::vec3(0.0f, 0.0f, 0.0f);
    euclidean_camera.transform().lookAt(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto camera = HyperCamera(HyperbolicProjection::BeltramiKlein);
    // euclidean_camera.transform().position() = glm::vec3(0, 0, 1);
    // euclidean_camera.transform().lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    // auto camera = HyperCamera(HyperbolicProjection::PoincareDisk);

    camera.eyeOffset() = glm::vec3(0.0f, 0.0f, .15f);

    renderer.useProgram();

    renderer.setCamera(camera);
    renderer.setEuclideanCamera(euclidean_camera);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.25f, .75f);

    auto grid = generateHypergrid(distance, size);
    std::vector<glm::vec3> colors;
    // for (auto& transform : grid) {
    //     colors.push_back(glm::vec3(dis(gen), dis(gen), dis(gen)));
    // }
    colors.push_back({0, .666, 0});
    glClearColor(.5, .75, 1, 1);

    FPSController controller;
    controller.pitch() = glm::radians(90.0f);

    glm::vec3 velocity(0.0f);
    bool locked = false;

    auto callback_data = CallbackData {velocity, locked};
    handleCallbacks(window, callback_data);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_NONE);
    glLineWidth(2);

    auto start = glfwGetTime();
    auto last = start;
    while (!window.shouldClose()) {
        auto now = glfwGetTime();
        auto time = now - start;
        auto delta = now - last;

        if (locked) {
            glfwSetInputMode(window.handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            controller.rotate(-callback_data.mouse_delta);
        } else {
            glfwSetInputMode(window.handle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        euclidean_camera.transform().rotation() = glm::vec3(0.0f, 0.0f, 0.0f);
        euclidean_camera.transform().rotate(glm::vec3(0, 0, 1), controller.yaw());
        euclidean_camera.transform().rotate(glm::vec3(1, 0, 0), controller.pitch());
        auto rotated_velocity = glm::vec3(controller.yawMatrix() * glm::vec4(velocity.x, velocity.y, 0, 1));
        camera.transform().translate(float(delta) * rotated_velocity * speed);
        camera.eyeOffset() += glm::vec3(0, 0, velocity.z * speed) * float(delta);

        renderer.setEuclideanCamera(euclidean_camera);
        renderer.setCamera(camera);

        renderer.clear();
        // int i = 0;
        renderer.setColor(glm::vec3(.25f));
        renderer.setTexture(grass);
        for (auto& transform : grid) {
            // auto color = colors[i % colors.size()];
            // renderer.setColor(color);
            renderer.render(hyper_mesh, transform);
            // i++;
        }

        auto T = HyperTransform() //
                                  //  .translated(glm::vec3(0, 0, 0.0001f))             //
                                  //  .rotated(glm::vec3(0, 1, 0), glm::radians(90.0f)) //
            ;
        renderer.setTexture(std::nullopt);
        renderer.setColor(glm::vec3(1.f));
        renderer.render(hyper_tree, T);

        callback_data.updateDeltas();
        window.pollEvents();
        window.swapBuffers();
        last = now;
    }

    return 0;
}
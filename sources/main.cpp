#include "HyperMesh.hpp"
#include "HyperTransform.hpp"
#include "data.hpp"
#include "misc.hpp"
#include "shapes.hpp"
#include "toto-engine/loader/shader.hpp"
#include "toto-engine/utils/camera.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <string>
#include <toto-engine/gl/glresources.hpp>
#include <toto-engine/mesh.hpp>
#include <toto-engine/uniform.hpp>
#include <toto-engine/window.hpp>
#include <vector>

int main(int argc, const char* argv[]) {
    int width = 1440;
    int height = 900;
    float aspect = static_cast<float>(width) / static_cast<float>(height);

    auto window = toto::Window(width, height, "Hyperbolic");
    window.makeContextCurrent();
    toto::Window::initGL();

    // const float size = 1.f;
    const float size = tileSizeEucl(5) * 2.;
    const float distance = tileDistance(5);

    auto hyper_plane = plane(size, size, 5);
    hyper_plane.vertices = hyperbolize(hyper_plane.vertices, 5);
    auto hyper_mesh = HyperMesh(hyper_plane.vertices, hyper_plane.indices, GL_TRIANGLES);

    auto camera_transform = HyperTransform();

    auto shader = toto::loadRenderShaderFile("resources/hyper.vert", "resources/hyper.frag");

    auto u_projection_euclidean = toto::Uniform(shader, "u_projection_euclidean");
    auto u_view_euclidean = toto::Uniform(shader, "u_view_euclidean");
    auto u_projection_mode = toto::Uniform(shader, "u_projection_mode");
    auto u_view = toto::Uniform(shader, "u_view");
    auto u_model = toto::Uniform(shader, "u_model");
    auto u_color = toto::Uniform(shader, "u_color");

    auto euclidean_camera = toto::Camera::Perspective(glm::radians(70.0f), aspect, 0.1f, 100.0f);

    // euclidean_camera.transform().position() = glm::vec3(0.0f, 0.0f, 2.0f);
    // euclidean_camera.transform().lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    euclidean_camera.transform().position() = glm::vec3(0.0f, 0.0f, 0.0f);
    euclidean_camera.transform().lookAt(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    shader.use();

    u_projection_euclidean.set(euclidean_camera.projectionMatrix());
    u_view_euclidean.set(euclidean_camera.viewMatrix());
    // u_view_euclidean.set(glm::lookAt(glm::vec3(0.0f, -2.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    u_projection_mode.set(0); // Beltrami-Klein
    // u_projection_mode.set(1); // Poincaré disk
    u_view.set(glm::mat4(1.0f));
    u_model.set(glm::mat4(1.0f));

    glm::vec3 velocity(0.0f);
    bool is_moving = false;

    glm::vec3 colors[] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 0},
        {1, 0, 1},
        {0, 1, 1},
        {1, 1, 1},
    };

    auto callback_data = CallbackData {velocity, is_moving};
    handleCallbacks(window, callback_data);

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_NONE);

    auto start = glfwGetTime();
    auto last = start;
    while (!window.shouldClose()) {
        auto now = glfwGetTime();
        auto time = now - start;
        auto delta = now - last;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera_transform.translate(float(delta) * velocity);

        shader.use();
        u_view.set(glm::inverse(camera_transform.matrix() * HyperTransform::translation(glm::vec3(0, 0, .25f))));

        u_color.set(colors[0]);
        u_model.set(glm::mat4(1.0f));
        hyper_mesh.draw();

        u_color.set(colors[1]);
        u_model.set(HyperTransform::translation(glm::vec3(1, 0, 0) * distance));
        hyper_mesh.draw();

        u_color.set(colors[2]);
        u_model.set(HyperTransform::translation(glm::vec3(-1, 0, 0) * distance));
        hyper_mesh.draw();

        u_color.set(colors[3]);
        u_model.set(HyperTransform::translation(glm::vec3(0, 1, 0) * distance));
        hyper_mesh.draw();

        u_color.set(colors[4]);
        u_model.set(HyperTransform::translation(glm::vec3(0, -1, 0) * distance));
        hyper_mesh.draw();

        window.pollEvents();
        window.swapBuffers();
        last = now;
    }

    return 0;
}
#include <toto-engine/gl/glresources.hpp>
#include <toto-engine/mesh.hpp>
#include <toto-engine/uniform.hpp>
#include <toto-engine/window.hpp>

int main(int argc, const char* argv[]) {
    auto window = toto::Window(1440, 900, "Hyperbolic");
    window.makeContextCurrent();
    toto::Window::initGL();

    glEnable(GL_DEPTH_TEST);

    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.pollEvents();
        window.swapBuffers();
    }

    return 0;
}
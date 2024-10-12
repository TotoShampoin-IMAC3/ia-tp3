#include "data.hpp"

void handleCallbacks(toto::Window& window, CallbackData& callback_data) {
    glfwSetWindowUserPointer(window.handle(), &callback_data);
    glfwSetKeyCallback(window.handle(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto callback_data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
        bool& left = callback_data->left;
        bool& right = callback_data->right;
        bool& up = callback_data->up;
        bool& down = callback_data->down;
        if (action == GLFW_PRESS) {
            switch (key) {
            case GLFW_KEY_LEFT: left = true; break;
            case GLFW_KEY_RIGHT: right = true; break;
            case GLFW_KEY_UP: up = true; break;
            case GLFW_KEY_DOWN: down = true; break;
            }
        } else if (action == GLFW_RELEASE) {
            switch (key) {
            case GLFW_KEY_LEFT: left = false; break;
            case GLFW_KEY_RIGHT: right = false; break;
            case GLFW_KEY_UP: up = false; break;
            case GLFW_KEY_DOWN: down = false; break;
            }
        }
        callback_data->velocity = glm::vec3(right - left, up - down, 0.0f);
    });
}

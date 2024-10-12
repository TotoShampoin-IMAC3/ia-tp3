#include "data.hpp"
#include "HyperTransform.hpp"
#include <GLFW/glfw3.h>
#include <vector>

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
            case GLFW_KEY_LEFT:
            case GLFW_KEY_A: {
                left = true;
            } break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_D: {
                right = true;
            } break;
            case GLFW_KEY_UP:
            case GLFW_KEY_W: {
                up = true;
            } break;
            case GLFW_KEY_DOWN:
            case GLFW_KEY_S: {
                down = true;
            } break;
            case GLFW_KEY_ESCAPE: {
                callback_data->locked = false;
            } break;
            }
        } else if (action == GLFW_RELEASE) {
            switch (key) {
            case GLFW_KEY_LEFT:
            case GLFW_KEY_A: {
                left = false;
            } break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_D: {
                right = false;
            } break;
            case GLFW_KEY_UP:
            case GLFW_KEY_W: {
                up = false;
            } break;
            case GLFW_KEY_DOWN:
            case GLFW_KEY_S: {
                down = false;
            } break;
            }
        }
        callback_data->velocity = glm::vec3(right - left, up - down, 0.0f);
    });
    glfwSetCursorPosCallback(window.handle(), [](GLFWwindow* window, double xpos, double ypos) {
        auto callback_data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
        auto& mouse_position = callback_data->mouse_position;
        auto& mouse_delta = callback_data->mouse_delta;
        mouse_delta = glm::vec2(xpos, ypos) - mouse_position;
        mouse_position = glm::vec2(xpos, ypos);
    });
    glfwSetMouseButtonCallback(window.handle(), [](GLFWwindow* window, int button, int action, int mods) {
        auto callback_data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            callback_data->locked = true;
        }
    });
}

std::vector<HyperTransform> generateHypergrid(const float& distance, const float& size) {
    return std::vector<HyperTransform> {
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
}

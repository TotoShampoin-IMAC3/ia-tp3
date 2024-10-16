#include "data.hpp"
#include "HyperTransform.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

void handleCallbacks(toto::Window& window, CallbackData& callback_data) {
    glfwSetWindowUserPointer(window.handle(), &callback_data);
    glfwSetWindowSizeCallback(window.handle(), [](GLFWwindow* window, int width, int height) {
        auto callback_data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        glViewport(0, 0, width, height);
        callback_data->camera.setPerspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
    });
    glfwSetKeyCallback(window.handle(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto callback_data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
        bool& left = callback_data->left;
        bool& right = callback_data->right;
        bool& up = callback_data->up;
        bool& down = callback_data->down;
        bool& space = callback_data->space;
        bool& shift = callback_data->shift;
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
            case GLFW_KEY_SPACE: {
                space = true;
            } break;
            case GLFW_KEY_LEFT_SHIFT: {
                shift = true;
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
            case GLFW_KEY_SPACE: {
                space = false;
            } break;
            case GLFW_KEY_LEFT_SHIFT: {
                shift = false;
            } break;
            }
        }
        callback_data->velocity = glm::vec3(right - left, up - down, space - shift);
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
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse) {
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

void initImgui(toto::Window& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void renderImgui(toto::Window& window, ImugiData& data) {
    static auto regen = [&] {
        data.rule.axiom = tostring(data.axiom_str);
        data.rule.nodraw = std::vector<char>(data.nodraw_str.begin(), data.nodraw_str.end());
        data.rule.setRules(data.rule.axiom, tostring(data.rules_str));
        data.rule.angle = data.angle;
        auto model = data.rule.generate(data.length, data.nb_iter);
        data.hyper_tree.set(model.vertices, model.indices);
    };

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("LSystem");
    bool changed = false;
    changed |= ImGui::InputText("Axiom", data.axiom_str.data(), data.axiom_str.size());
    changed |= ImGui::InputTextMultiline("Rules", data.rules_str.data(), data.rules_str.size());
    changed |= ImGui::InputText("No Draw", data.nodraw_str.data(), data.nodraw_str.size());
    changed |= ImGui::InputInt("Iterations", &data.nb_iter);
    changed |= ImGui::InputFloat("Angle", &data.angle);
    changed |= ImGui::InputFloat("Length", &data.length);
    if (changed && data.live_gen) {
        regen();
    }
    if (ImGui::Button("Regenerate")) {
        regen();
    }
    ImGui::SameLine();
    ImGui::Checkbox("Live Generation", &data.live_gen);
    ImGui::Text("");
    ImGui::Checkbox("Use Outside Camera", &data.outside_cam);
    ImGui::InputFloat3("Eye Offset", &data.eye_offset.x);
    if (ImGui::Button("Reset Position")) {
        data.camera.transform() = HyperTransform();
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

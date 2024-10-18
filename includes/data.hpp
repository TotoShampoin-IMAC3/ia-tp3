#pragma once

#include "HyperCamera.hpp"
#include "HyperMesh.hpp"
#include "HyperTransform.hpp"
#include "LSystem2.hpp"
#include "toto-engine/utils/camera.hpp"
#include "toto-engine/window.hpp"
#include <glm/glm.hpp>
#include <vector>

struct CallbackData {
    glm::vec3& velocity;
    bool& locked;
    toto::Camera& camera;
    bool left, right, up, down, space, shift;
    glm::vec2 mouse_position;
    glm::vec2 mouse_delta = glm::vec2(0.0f);

    void updateDeltas() { mouse_delta = glm::vec2(0.0f); }
};
struct ImugiData {
    LSystem& rule;
    HyperMesh& hyper_tree;
    bool& outside_cam;
    HyperCamera& camera;
    glm::vec3& eye_offset;
    bool& hide_floor;
    std::vector<char> axiom_str = std::vector<char>(256, 0);
    std::vector<char> rules_str = std::vector<char>(512, 0);
    std::vector<char> nodraw_str = std::vector<char>(256, 0);
    int nb_iter = 4;
    float angle = 0;
    float length = .025f;
    bool live_gen = false;

    void extractRule() {
        std::copy(rule.axiom.begin(), rule.axiom.end(), axiom_str.begin());
        angle = rule.angle;
    }
};

void handleCallbacks(toto::Window&, CallbackData&);

std::vector<HyperTransform> generateHypergrid(const float& distance, const float& size);

void initImgui(toto::Window&);
void renderImgui(toto::Window&, ImugiData&);

inline std::string tostring(const std::vector<char>& str) {
    return std::string(str.begin(), std::find(str.begin(), str.end(), '\0'));
}
inline std::vector<char> tochar(const std::string& str, size_t size = 256) {
    std::vector<char> res(size, 0);
    std::copy(str.begin(), str.end(), res.begin());
    return res;
}

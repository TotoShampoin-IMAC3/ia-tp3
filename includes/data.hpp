#pragma once

#include "HyperCamera.hpp"
#include "HyperMesh.hpp"
#include "HyperTransform.hpp"
#include "LSystem.hpp"
#include "toto-engine/window.hpp"
#include <glm/glm.hpp>
#include <vector>

struct CallbackData {
    glm::vec3& velocity;
    bool& locked;
    bool left, right, up, down, space, shift;
    glm::vec2 mouse_position;
    glm::vec2 mouse_delta = glm::vec2(0.0f);

    void updateDeltas() { mouse_delta = glm::vec2(0.0f); }
};
struct ImugiData {
    LSystemRule& rule;
    HyperMesh& hyper_tree;
    bool& outside_cam;
    HyperCamera& camera;
    glm::vec3& eye_offset;
    std::vector<char> rule_str = std::vector<char>(256, 0);
    int nb_iter = 4;
    float angle = 0;
    float length = 0;

    void extractRule() {
        std::copy(rule.path().begin(), rule.path().end(), rule_str.begin());
        angle = rule.angle();
        length = rule.length();
    }
};

void handleCallbacks(toto::Window&, CallbackData&);

std::vector<HyperTransform> generateHypergrid(const float& distance, const float& size);

void initImgui(toto::Window&);
void renderImgui(toto::Window&, ImugiData&);

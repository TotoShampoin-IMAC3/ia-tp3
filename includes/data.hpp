#pragma once

#include "HyperTransform.hpp"
#include "toto-engine/window.hpp"
#include <glm/glm.hpp>
#include <vector>

struct CallbackData {
    glm::vec3& velocity;
    bool& locked;
    bool left, right, up, down;
    glm::vec2 mouse_position;
    glm::vec2 mouse_delta = glm::vec2(0.0f);

    void updateDeltas() { mouse_delta = glm::vec2(0.0f); }
};

void handleCallbacks(toto::Window&, CallbackData&);

std::vector<HyperTransform> generateHypergrid(const float& distance, const float& size);

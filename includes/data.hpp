#pragma once

#include "toto-engine/window.hpp"
#include <glm/glm.hpp>

struct CallbackData {
    glm::vec3& velocity;
    bool left, right, up, down;
};

void handleCallbacks(toto::Window&, CallbackData&);

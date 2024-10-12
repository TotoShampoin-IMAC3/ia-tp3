#pragma once

#include "HyperMesh.hpp"

/**
 * @brief A subdivided plane
 */
HyperModel plane(float width, float height, int subdivs);

/**
 * @brief A subdivided cube
 */
HyperModel cube(float width, float height, float depth, int subdivs);

#pragma once
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <string>
#include <vector>

/**
 * @brief The size of a square tile in H3 hyperbolic space.
 *
 */
inline float tileSizeEucl(int squares_at_a_vertex) {
    float angle = glm::pi<float>() / squares_at_a_vertex;
    float leg = glm::pow(glm::cos(angle), 2) - 0.5;
    return glm::sqrt(leg) / glm::cos(angle);
}

/**
 * @brief The distance between the centers of two adjacent tiles in H3 hyperbolic space.
 *
 */
inline float tileDistance(int squares_at_a_vertex) {
    float angle = glm::pi<float>() / squares_at_a_vertex;
    float leg = glm::cos(angle) / glm::sqrt(.5);
    return 2 * glm::acosh(leg);
}

// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
inline std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

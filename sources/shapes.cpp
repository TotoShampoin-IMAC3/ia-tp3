#include "shapes.hpp"
#include "HyperMesh.hpp"
#include <cstddef>

HyperModel plane(float width, float height, int subdivs) {
    auto vertices = std::vector<HyperVertex>();
    auto indices = std::vector<uint>();

    for (int i = 0; i <= subdivs; i++) {
        for (int j = 0; j <= subdivs; j++) {
            vertices.push_back({
                {width * (j / static_cast<float>(subdivs) - 0.5f), height * (i / static_cast<float>(subdivs) - 0.5f),
                 0.f, 1.0f},
                {j / static_cast<float>(subdivs), i / static_cast<float>(subdivs)}
            });
        }
    }

    for (int i = 0; i < subdivs; i++) {
        for (int j = 0; j < subdivs; j++) {
            indices.push_back(i * (subdivs + 1) + j);
            indices.push_back(i * (subdivs + 1) + j + 1);
            indices.push_back((i + 1) * (subdivs + 1) + j + 1);
            indices.push_back(i * (subdivs + 1) + j);
            indices.push_back((i + 1) * (subdivs + 1) + j + 1);
            indices.push_back((i + 1) * (subdivs + 1) + j);
        }
    }

    return HyperModel {vertices, indices};
}

HyperModel cube(float width, float height, float depth, int subdivs) {
    std::vector<HyperVertex> vertices;
    std::vector<uint> indices;
    size_t idx = 0;

    auto planeWH = plane(width, height, subdivs);
    auto planeWD = plane(width, depth, subdivs);
    auto planeDH = plane(depth, height, subdivs);

    for (auto& vertex : planeWH.vertices) {
        vertices.push_back({
            {vertex.position.x, vertex.position.y, -depth / 2, 1.0f},
            vertex.texcoord
        });
    }
    for (auto& index : planeWH.indices) {
        indices.push_back(index + idx);
    }
    idx += planeWH.vertices.size();

    for (auto& vertex : planeWH.vertices) {
        vertices.push_back({
            {vertex.position.x, vertex.position.y, depth / 2, 1.0f},
            vertex.texcoord
        });
    }
    for (auto& index : planeWH.indices) {
        indices.push_back(index + idx);
    }
    idx += planeWH.vertices.size();

    for (auto& vertex : planeWD.vertices) {
        vertices.push_back({
            {vertex.position.x, -height / 2, vertex.position.y, 1.0f},
            vertex.texcoord
        });
    }
    for (auto& index : planeWD.indices) {
        indices.push_back(index + idx);
    }
    idx += planeWD.vertices.size();

    for (auto& vertex : planeWD.vertices) {
        vertices.push_back({
            {vertex.position.x, height / 2, vertex.position.y, 1.0f},
            vertex.texcoord
        });
    }
    for (auto& index : planeWD.indices) {
        indices.push_back(index + idx);
    }
    idx += planeWD.vertices.size();

    for (auto& vertex : planeDH.vertices) {
        vertices.push_back({
            {-width / 2, vertex.position.y, vertex.position.x, 1.0f},
            vertex.texcoord
        });
    }
    for (auto& index : planeDH.indices) {
        indices.push_back(index + idx);
    }
    idx += planeDH.vertices.size();

    for (auto& vertex : planeDH.vertices) {
        vertices.push_back({
            {width / 2, vertex.position.y, vertex.position.x, 1.0f},
            vertex.texcoord
        });
    }
    for (auto& index : planeDH.indices) {
        indices.push_back(index + idx);
    }

    return HyperModel {vertices, indices};
}

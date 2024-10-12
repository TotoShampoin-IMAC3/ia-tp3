#version 460 core

#define BELTRAMI 0
#define POINCARE 1
#define GANS 2

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texcoord;

uniform mat4 u_projection_euclidean;
uniform mat4 u_view_euclidean;

uniform int u_projection_mode;
uniform mat4 u_view;
uniform mat4 u_model;

out vec2 v_texcoord;

vec3 hyperbolicProjection(vec4 point, int mode) {
    if(mode == BELTRAMI) {
        return point.xyz / point.w;
    } else if(mode == POINCARE) {
        return point.xyz / (1.0 + point.w);
    } else if(mode == GANS) {
        return point.xyz;
    }
}

void main() {
    vec4 position = u_view * u_model * a_position;
    position = vec4(hyperbolicProjection(position, u_projection_mode), 1.0);
    gl_Position = u_projection_euclidean * u_view_euclidean * position;
    v_texcoord = a_texcoord;
}

#version 460 core

uniform vec3 u_color;
uniform sampler2D u_texture;
uniform bool u_use_texture;

in vec2 v_texcoord;

out vec4 f_color;

void main() {
    f_color = vec4(u_color, 1.0);
    if(u_use_texture) {
        f_color = f_color * texture(u_texture, v_texcoord);
    }
}

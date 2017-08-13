#version 330 core

in vec2 tex_coord;

out vec3 color;

uniform sampler2D TEX; // texture uniform

void main() {
    color = texture(TEX, tex_coord).rgb;
}

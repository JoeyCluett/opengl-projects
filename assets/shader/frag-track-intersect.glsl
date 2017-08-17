#version 330 core

in  vec4 intersect_loc;
in  vec2 tex_coord;
out vec3 color;

uniform sampler2D TEX; // texture uniform

void main() {
    float dist = distance(gl_FragCoord, intersect_loc);

    if(dist < 30.0)
        color = vec3(1.0, 0, 0);
    else
        color = texture(TEX, tex_coord).rgb;
}


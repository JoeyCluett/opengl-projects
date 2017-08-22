#version 330 core

in  vec4 intersect_loc;
in  vec2 tex_coord;
out vec3 color;

uniform sampler2D TEX; // texture uniform

void main() {
    float w = gl_FragColor.w;
    float x = gl_FragCoord.x;// / w;
    float y = gl_FragCoord.y;// / w;
    float z = gl_FragCoord.z;// / w;

    float _x = intersect_loc.x - x;
    float _y = intersect_loc.y - y;
    float _z = intersect_loc.z - z;

    float dist = sqrt(_x*_x + _y*_y + _z*_z);

    if(dist < 10.0)
        color = vec3(1.0, 0, 0);
    else
        color = texture(TEX, tex_coord).rgb;
}


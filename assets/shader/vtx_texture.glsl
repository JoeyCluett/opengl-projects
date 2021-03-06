#version 330 core

uniform mat4 mat4_extra_0;
uniform mat4 mat4_extra_1;
uniform mat4 mat4_extra_2;

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec2 vertexUV;

out vec2 tex_coord;

// standard data needed for vertex transformation
uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    tex_coord = vertexUV;
}

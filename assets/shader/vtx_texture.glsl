#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec2 vertexUV;

out vec2 tex_coord;
uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
    tex_coord = vertexUV;
}

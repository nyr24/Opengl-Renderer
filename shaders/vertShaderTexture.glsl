#version 330

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_tex_coord;

uniform mat4 u_mvp_mat;

out vec2 tex_coord;

void main() {
    gl_Position = u_mvp_mat * vec4(a_position, 1.0);
    tex_coord = a_tex_coord;
}

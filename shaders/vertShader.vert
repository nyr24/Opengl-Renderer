#version 330

layout(location = 0) in vec3 a_position;
//layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_tex_coord;

uniform mat4 u_mvp_mat;

//flat out vec4 passed_color;
out vec2 tex_coord;

void main() {
    gl_Position = u_mvp_mat * vec4(a_position, 1.0);
    //passed_color = a_color;
    tex_coord = a_tex_coord;
}
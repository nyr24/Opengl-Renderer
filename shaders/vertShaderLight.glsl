#version 330

layout(location = 0) in vec3 a_pos;

uniform mat4 u_mvp_mat;

void main() {
    gl_Position = u_mvp_mat * vec4(a_pos, 1.0);
}

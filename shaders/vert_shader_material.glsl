#version 330

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;

uniform mat4 u_model_view_mat;
uniform mat4 u_normal_mat;
uniform mat4 u_mvp_mat;

smooth  out vec3 passed_normal;
smooth  out vec3 passed_frag_pos;

void main() {
    vec4 a_pos_homogen      =   vec4(a_pos, 1.0);
    gl_Position             =   u_mvp_mat * a_pos_homogen;
    passed_frag_pos         =   vec3(u_model_view_mat * a_pos_homogen);
    passed_normal           =   mat3(u_normal_mat) * a_normal;
}

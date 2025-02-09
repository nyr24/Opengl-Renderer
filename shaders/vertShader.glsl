#version 330

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec3 a_normal;

uniform mat4 u_model_mat;
// uniform mat4 u_normal_mat;
uniform mat4 u_mvp_mat;

flat    out vec3 passed_color;
smooth  out vec3 passed_normal;
smooth  out vec3 passed_frag_pos;

void main() {
    vec4 a_pos_homogen      =   vec4(a_pos, 1.0);
    // TODO: need we inverse + transpose model + view matrix, if we want to calc light in View coordinates ?
    gl_Position             =   u_mvp_mat * a_pos_homogen;
    passed_frag_pos         =   vec3(u_model_mat * a_pos_homogen);
    passed_color            =   a_color;
    mat3 normal_mat         =   mat3(inverse(transpose(u_model_mat)));
    passed_normal           =   normal_mat * a_normal;
}

#version 330

layout (location = 0) in vec4 position;

uniform mat4 u_translation_mat;
uniform mat4 u_rotation_mat;
uniform mat4 u_scaling_mat;

void main() {
    mat4 local_mat = u_translation_mat * u_rotation_mat * u_scaling_mat;
    gl_Position = local_mat * position;
}
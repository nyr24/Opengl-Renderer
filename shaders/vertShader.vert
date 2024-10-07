#version 330

layout (location = 0) in vec4 position;

//uniform mat4 u_translation_mat;
//uniform mat4 u_scaling_mat;
uniform mat4 u_rotation_mat;

void main() {
    gl_Position = u_rotation_mat * position;
}
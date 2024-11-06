#version 330

layout(location = 0) in vec3 position;
//layout(location = 1) in vec4 color;

//uniform mat4 u_mvp_mat;
uniform mat4 u_proj_mat;
uniform vec2 u_move_offset;

//flat out vec4 passed_color;

void main() {
    //gl_Position = u_mvp_mat * vec4(position, 1.0);
    //passed_color = color;

    gl_PointSize = 20.0f;

    vec4 position = vec4(position.x + u_move_offset.x, position.y + u_move_offset.y, position.z, 1.0f);

    gl_Position = u_proj_mat * position;
}
#version 330

//flat in vec4 passed_color;
in vec2 tex_coord;

out vec4 output_color;

uniform sampler2D u_texture_data;

void main() {
    //output_color = passed_color;
    output_color = texture2D(u_texture_data, tex_coord);
}
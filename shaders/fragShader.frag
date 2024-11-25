#version 330

//flat in vec4 passed_color;
in vec2 tex_coord;

out vec4 output_color;

uniform sampler2D u_tex_data1;
uniform sampler2D u_tex_data2;

void main() {
    //output_color = passed_color;
    output_color = mix(
        texture2D(u_tex_data1, tex_coord), 
        texture2D(u_tex_data2, tex_coord), 0.8
    );
}
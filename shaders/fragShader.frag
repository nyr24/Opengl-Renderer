#version 330

//flat in vec4 passed_color;

out vec4 output_color;

//uniform float u_elapsed_time;
//uniform float u_loop_duration;

//const vec4 color_one = vec4(0.0f, 0.0f, 1.0f, 1.0f);
//const vec4 color_two = vec4(0.0f, 1.0f, 0.0f, 1.0f);

void main() {
    output_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    //output_color = passed_color;
}
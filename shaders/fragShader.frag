#version 330

out vec4 output_color;

uniform float u_elapsed_time;
uniform float u_loop_duration;

const vec4 color_one = vec4(0.0f, 0.0f, 1.0f, 1.0f);
const vec4 color_two = vec4(0.0f, 1.0f, 0.0f, 1.0f);

void main() {
    float from_0_to_loop_dur = mod(u_elapsed_time, u_loop_duration);
    float from_0_to_1 = from_0_to_loop_dur / u_loop_duration;
    float lerp_val = sin(from_0_to_1);

    output_color = mix(color_one, color_two, lerp_val);
}
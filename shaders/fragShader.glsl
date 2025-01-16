#version 330

flat in vec4 passed_color;

out vec4 output_color;

uniform float u_lerp;

void main() {
    output_color = vec4(passed_color.xyz * u_lerp, passed_color.w);
}

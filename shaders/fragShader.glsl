#version 330

flat in vec4 passed_color;

out vec4 output_color;

void main() {
    output_color = passed_color;
}

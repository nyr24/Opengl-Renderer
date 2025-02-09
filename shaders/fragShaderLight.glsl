#version 330

out vec4 output_color;

uniform vec3 u_color;

void main() {
    output_color = vec4(u_color, 1.0);
}

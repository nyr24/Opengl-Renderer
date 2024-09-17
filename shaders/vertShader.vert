#version 330

layout(location = 0) in vec4 position;

uniform vec2 u_offset;

void main(){
    gl_Position = position + vec4(u_offset, 0.0f, 1.0f);
}
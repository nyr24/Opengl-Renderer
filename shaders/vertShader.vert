#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

uniform vec2 u_offset;

smooth out vec4 vertexColor;

void main(){
    gl_Position = position + vec4(u_offset, 0.0f, 1.0f);
    vertexColor = color;
}
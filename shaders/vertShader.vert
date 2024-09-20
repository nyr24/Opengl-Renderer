#version 330

layout(location = 0) in vec4 position;

uniform float u_elapsedTime;
uniform float u_loopDuration;

const float PI = 3.141592f;

void main(){
    float scale = PI * 2 / u_loopDuration;
    float currTime = mod(u_elapsedTime, u_loopDuration);
    vec2 offset = vec2(
        cos(scale * currTime) * 1.25f,
        sin(scale * currTime) * 1.25f
    );

    gl_Position = position + vec4(offset, 0.0f, 1.0f);
}
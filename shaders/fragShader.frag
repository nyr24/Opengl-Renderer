#version 330

out vec4 outColor;

uniform float u_elapsedTime;
uniform float u_loopDuration;

const vec4 colorOne = vec4(0.0f, 0.0f, 1.0f, 1.0f);
const vec4 colorTwo = vec4(0.0f, 1.0f, 0.0f, 1.0f);

void main() {
    float currTime = mod(u_elapsedTime, u_loopDuration);
    float lerpValue = currTime / u_loopDuration;

    outColor = mix(colorOne, colorTwo, lerpValue);
}
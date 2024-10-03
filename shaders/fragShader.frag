#version 330

out vec4 outputColor;

uniform float u_window_height;

const vec4 colorOne = vec4(0.0f, 0.0f, 1.0f, 1.0f);
const vec4 colorTwo = vec4(0.0f, 1.0f, 0.0f, 1.0f);

void main() {
    outputColor = mix(colorOne, colorTwo, gl_FragCoord / u_window_height);
}

/* 


    #version 330

out vec4 outColor;

uniform float u_elapsedTime;
uniform float u_loopDuration;

const vec4 colorOne = vec4(0.0f, 0.0f, 1.0f, 1.0f);
const vec4 colorTwo = vec4(0.0f, 1.0f, 0.0f, 1.0f);

void main() {
    float currTime = mod(u_elapsedTime, u_loopDuration);
    float lerpValue = sin(currTime);

    outColor = mix(colorOne, colorTwo, lerpValue);
}
 */
#version 330

out vec4 outColor;

uniform vec4 u_colorOne;
uniform vec4 u_colorTwo;
uniform float u_windowHeight;

void main() {
    float lerpValue = gl_FragCoord.y / u_windowHeight;

    outColor = mix(u_colorOne, u_colorTwo, lerpValue);
}
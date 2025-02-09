#version 330

flat    in vec3 passed_color;
smooth  in vec3 passed_normal;
smooth  in vec3 passed_frag_pos;

out vec4 output_color;

uniform vec3 u_light_pos;
uniform vec3 u_light_color;
// uniform float u_lerp;

void main() {
    float ambient_coef       = 0.08;
    vec3  light_dir          = normalize(u_light_pos - passed_frag_pos);
    float diffuse_coef       = max(dot(light_dir, passed_normal), 0.0);
    vec3  light_color_result = u_light_color * (ambient_coef + diffuse_coef);
    output_color             = vec4(passed_color * light_color_result, 1.0);
}

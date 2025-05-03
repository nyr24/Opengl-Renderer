#version 330

flat    in vec3 passed_color;
smooth  in vec3 passed_normal;
smooth  in vec3 passed_frag_pos;

out vec4 output_color;

uniform vec3 u_light_pos;
uniform vec3 u_light_color;
uniform vec3 u_view_pos;
// uniform float u_lerp;

void main() {
    float   ambient_coef        = 0.1;
    float   shininess           = 16;
    float   specular_intensity  = 0.5;

    vec3    normal_normalized   = normalize(passed_normal);
    vec3    light_dir           = normalize(u_light_pos - passed_frag_pos);
    vec3    view_dir            = normalize(u_view_pos - passed_frag_pos);
    vec3    reflect_dir         = reflect(-light_dir, normal_normalized);
    float   diffuse_coef        = max(dot(light_dir, normal_normalized), 0.0);
    float   specular_coef       = pow(max(dot(reflect_dir, view_dir), 0.0), shininess) * specular_intensity;
    vec3    light_color_result  = u_light_color * (ambient_coef + diffuse_coef + specular_coef);
    output_color                = vec4(passed_color * light_color_result, 1.0);
}

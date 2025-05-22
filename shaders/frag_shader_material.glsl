#version 330

smooth in vec3 passed_normal;
smooth in vec3 passed_frag_pos;

out vec4 output_color;

uniform vec3 u_light_pos;
uniform vec3 u_light_color;
uniform vec3 u_view_pos;
// uniform float u_lerp;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material u_material;

void main() {
    // ambient
    vec3 ambient_vec = u_light_color * u_material.ambient;

    // diffuse
    vec3 normal_normalized = normalize(passed_normal);
    vec3 light_dir = normalize(u_light_pos - passed_frag_pos);
    float diffuse_coef = max(dot(light_dir, normal_normalized), 0.0);
    vec3 diffuse_vec = u_light_color * (u_material.diffuse * diffuse_coef);

    // specular
    vec3 view_dir = normalize(u_view_pos - passed_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal_normalized);
    float specular_coef = pow(max(dot(reflect_dir, view_dir), 0.0), u_material.shininess);
    vec3 spectular_vec = u_light_color * (u_material.specular * specular_coef);

    vec3 light_color_result = ambient_vec + diffuse_vec + spectular_vec;
    output_color = vec4(light_color_result, 1.0);
}

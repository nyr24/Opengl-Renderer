#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <ctime>
#include "animation.hpp"
#include "math.hpp"
#include "matrix.hpp"
#include "sharedTypes.hpp"
#include "vec.hpp"
#include "utils.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "geometryObject.hpp"
#include "texture.hpp"
#include "globals.hpp"
#include "camera.hpp"
#include "meshes.hpp"
#include "userDefinedObjects.hpp"

int main() {
    my_gl::Window window{ my_gl::init_window() };

    constexpr uint16_t texture_offset{ sizeof(float) * 3 * 4 * 6 };
    constexpr uint16_t color_offset{ texture_offset + sizeof(float) * 2 * 4 * 6 };

    my_gl::Program program1{
        "shaders/vertShader.glsl",
        "shaders/fragShader.glsl",
        {
            { .name = "a_position", .gl_type = GL_FLOAT, .count = 3, .byte_stride = 0, .byte_offset = 0 },
            { .name = "a_color", .gl_type = GL_FLOAT, .count = 4, .byte_stride = 0, .byte_offset = color_offset },
        },
        {
            { .name = "u_mvp_mat" },
        }
    };

    my_gl::Program program2{
        "shaders/vertShaderTexture.glsl",
        "shaders/fragShaderTexture.glsl",
        {
            { .name = "a_position", .gl_type = GL_FLOAT, .count = 3, .byte_stride = 0, .byte_offset = 0 },
            { .name = "a_tex_coord", .gl_type = GL_FLOAT, .count = 2, .byte_stride = 0, .byte_offset = texture_offset }
        },
        {
            { .name = "u_mvp_mat" },
            { .name = "u_tex_data1" },
            { .name = "u_tex_data2" },
            { .name = "u_lerp" }
        }
    };

// move this to object to dynamically assign uniform value,
//this would be overwritten if specified more textures than uniforms
    std::vector<my_gl::Texture> textures = {
        { "res/mine_red.jpg", program2, program2.get_uniform("u_tex_data1"), 0, GL_TEXTURE0 },
        { "res/mine_green.jpg", program2, program2.get_uniform("u_tex_data2"), 1, GL_TEXTURE1 },
    };

    std::vector<const my_gl::Program*> programs{ &program1, &program2 };

    my_gl::VertexArray vertex_arr{
        my_gl::meshes::cube_mesh,
        programs
    };

    std::vector<my_gl_math::Matrix44<float>> cube1_transforms = {
        my_gl_math::Matrix44<float>::translation({ 0.4f, 1.0f, 0.0f }),
        my_gl_math::Matrix44<float>::rotation(90.0f, my_gl_math::Global::AXIS::Z)
    };

    std::vector<my_gl::Animation<float>> cube1_anims = {
        {
            my_gl::TRANSLATE,
            5.0f,
            0.0f,
            { -1.0f, 1.0f, 0.0f },
            { -1.0f, -1.0f, 0.0f },
            my_gl::Bezier_curve_type::EASE_IN,
            my_gl::Loop_type::INVERT
        },
        {
            0.0f,
            360.0f,
            5.0f,
            0.0f,
            my_gl_math::Global::AXIS::Y,
            my_gl::Bezier_curve_type::EASE_IN_OUT,
            my_gl::Loop_type::INVERT
        },
    };

    std::vector<my_gl_math::Matrix44<float>> cube2_transforms = {
        my_gl_math::Matrix44<float>::rotation(90.0f, my_gl_math::Global::AXIS::Z),
        my_gl_math::Matrix44<float>::translation({ 0.4f, 1.0f, 0.0f })
    };

    std::vector<my_gl::Animation<float>> cube2_anims = {
        {
            0.0f,
            360.0f,
            5.0f,
            0.0f,
            my_gl_math::Global::AXIS::Y,
            my_gl::Bezier_curve_type::EASE_IN_OUT,
            my_gl::Loop_type::INVERT
        },
        {
            my_gl::TRANSLATE,
            5.0f,
            0.0f,
            { 1.0f, 1.0f, 0.0f },
            { 1.0f, -1.0f, 0.0f },
            my_gl::Bezier_curve_type::EASE_IN,
            my_gl::Loop_type::INVERT
        },
    };

    /*std::vector<my_gl::GeometryObjectPrimitive> primitives{  };*/

    std::vector<my_gl::GeometryObjectComplex> complex_objs{{
        my_gl::create_cube_creature(program1, vertex_arr)
    }};

    auto view_mat{ my_gl_math::Matrix44<float>::look_at(
        my_gl::camera.camera_pos,
        my_gl::camera.camera_pos + my_gl::camera.camera_front,
        my_gl::camera.camera_up
    )};

    auto projection_mat{ my_gl_math::Matrix44<float>::perspective_fov(
        my_gl::camera.fov, my_gl::camera.aspect, 0.1f, 50.0f
    )};

    auto projection_view_mat{ projection_mat * view_mat };

    my_gl::Renderer renderer{
        std::move(complex_objs),
        std::vector<my_gl::GeometryObjectPrimitive>{},
        std::move(projection_view_mat),
    };

    glfwSwapInterval(1);

    bool is_rendering_started{false};
    while (!glfwWindowShouldClose(window.ptr_raw())) {
        auto start_frame{ std::chrono::steady_clock::now() };
        if (!is_rendering_started) {
            renderer.set_start_time(start_frame);
            is_rendering_started = true;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view_mat = my_gl_math::Matrix44<float>::look_at(
            my_gl::camera.camera_pos,
            my_gl::camera.camera_pos + my_gl::camera.camera_front,
            my_gl::camera.camera_up
        );
        projection_mat = my_gl_math::Matrix44<float>::perspective_fov(
            my_gl::camera.fov, my_gl::camera.aspect, 0.1f, 50.0f
        );

        projection_view_mat = projection_mat * view_mat;
        renderer.set_world_matrix(projection_view_mat);

        float time_0to1 = my_gl_math::Global::map_duration_to01(renderer.get_curr_rendering_duration());
        renderer.render(time_0to1);

        glfwSwapBuffers(window.ptr_raw());
        glfwPollEvents();

        my_gl::Duration_sec frame_duration{ std::chrono::steady_clock::now() - start_frame };
        renderer.update_time(frame_duration);
        my_gl::globals::delta_time = frame_duration.count();
    }

    return 0;
}

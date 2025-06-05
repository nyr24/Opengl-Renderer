#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "geometryObject.hpp"
#include "math.hpp"
#include "matrix.hpp"
#include "sharedTypes.hpp"
#include "utils.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "appCode.hpp"
#include "globals.hpp"
#include "camera.hpp"
#include "meshes.hpp"

int main() {
    my_gl::Window window{ my_gl::init_window() };

    constexpr uint16_t texture_offset{ sizeof(float) * 3 * 4 * 6 };
    constexpr uint16_t color_offset{ texture_offset + sizeof(float) * 2 * 4 * 6 };
    constexpr uint16_t normal_offset{ color_offset + sizeof(float) * 3 * 4 * 6 };

    my_gl::Program world_shader{
        "shaders/vert_shader_material.glsl",
        "shaders/frag_shader_material.glsl",
        {
            { .name = "a_pos", .gl_type = GL_FLOAT, .count = 3, .byte_stride = 0, .byte_offset = 0 },
            { .name = "a_normal", .gl_type = GL_FLOAT, .count = 3, .byte_stride = 0, .byte_offset = normal_offset },
        },
        {
            { .name = "u_mvp_mat" },
            { .name = "u_model_view_mat" },
            { .name = "u_normal_mat" },
            { .name = "u_view_pos" },
            { .name = "u_material.ambient" },
            { .name = "u_material.diffuse" },
            { .name = "u_material.specular" },
            { .name = "u_material.shininess" },
            { .name = "u_light.position" },
            { .name = "u_light.ambient" },
            { .name = "u_light.diffuse" },
            { .name = "u_light.specular" },
        }
    };

    my_gl::Program light_shader{
        "shaders/vert_shader_light.glsl",
        "shaders/frag_shader_light.glsl",
        {
            { .name = "a_pos", .gl_type = GL_FLOAT, .count = 3, .byte_stride = 0, .byte_offset = 0 },
        },
        {
            { .name = "u_mvp_mat" },
            { .name = "u_color" }
        }
    };

    // std::array<my_gl::Texture, 2> textures = {
    //    { "res/mine_red.jpg", program2, program2.get_uniform("u_tex_data1"), 0, GL_TEXTURE0 },
    //    { "res/mine_green.jpg", program2, program2.get_uniform("u_tex_data2"), 1, GL_TEXTURE1 },
    // };

    my_gl::VertexArray vertex_arr_world{
        my_gl::meshes::get_cube_mesh(),
        world_shader
    };

    my_gl::VertexArray vertex_arr_light{
        my_gl::meshes::get_cube_mesh(),
        light_shader
    };

    // transformations
    std::array transforms = {
        // ball
        my_gl::TransformData{
            my_gl::math::TransformationType::TRANSLATION,
            {
                my_gl::math::Matrix44<float>::translation({ 0.0f, 0.0f, 0.0f }),
            },
            {}
        },
        my_gl::TransformData{
            my_gl::math::TransformationType::SCALING,
            {
                my_gl::math::Matrix44<float>::scaling({ 1.3f, 0.6f, 1.0f }),
            },
            {}
        },

        // paddle
        my_gl::TransformData{
            my_gl::math::TransformationType::TRANSLATION,
            {
                my_gl::math::Matrix44<float>::translation({ 0.0f, -WALL_Y_OFFSET + 1.3f, 0.0f }),
            },
            {}
        },
        my_gl::TransformData{
            my_gl::math::TransformationType::SCALING,
            {
                my_gl::math::Matrix44<float>::scaling({ 1.0f, 0.3f, 1.0f }),
            },
            {}
        },

        // left wall
        my_gl::TransformData{
            my_gl::math::TransformationType::TRANSLATION,
            {
                my_gl::math::Matrix44<float>::translation({ -WALL_X_OFFSET, 0.0f, 0.0f })
            },
            {}
        },
        my_gl::TransformData{
            my_gl::math::TransformationType::SCALING,
            {
                my_gl::math::Matrix44<float>::scaling({ WALL_HEIGHT, WALL_WIDTH, 1.0f }),
            },
            {}
        },

        // right wall
        my_gl::TransformData{
            my_gl::math::TransformationType::TRANSLATION,
            {
                my_gl::math::Matrix44<float>::translation({ WALL_X_OFFSET, 0.0f, 0.0f })
            },
            {}
        },
        my_gl::TransformData{
            my_gl::math::TransformationType::SCALING,
            {
                my_gl::math::Matrix44<float>::scaling({ WALL_HEIGHT, WALL_WIDTH, 1.0f }),
            },
            {}
        },

        // top wall
        my_gl::TransformData{
            my_gl::math::TransformationType::TRANSLATION,
            {
                my_gl::math::Matrix44<float>::translation({ 0.0f, WALL_Y_OFFSET, 0.0f })
            },
            {}
        },
        my_gl::TransformData{
            my_gl::math::TransformationType::SCALING,
            {
                my_gl::math::Matrix44<float>::scaling({ WALL_WIDTH, WALL_HEIGHT, 1.0f }),
            },
            {}
        },

        // bottom wall
        my_gl::TransformData{
            my_gl::math::TransformationType::TRANSLATION,
            {
                my_gl::math::Matrix44<float>::translation({ 0.0f, -WALL_Y_OFFSET, 0.0f })
            },
            {}
        },
        my_gl::TransformData{
            my_gl::math::TransformationType::SCALING,
            {
                my_gl::math::Matrix44<float>::scaling({ WALL_WIDTH, WALL_HEIGHT, 1.0f }),
            },
            {}
        },

        // light
        my_gl::TransformData{
            my_gl::math::TransformationType::TRANSLATION,
            {
                my_gl::math::Matrix44<float>::translation(my_gl::globals::light.position),
                my_gl::math::Matrix44<float>::scaling({0.4f, 0.2f, 0.2f}),
            },
            {}
        }
    };

    // physics
    std::array physics = {
        // ball
        my_gl::Physics<float>{
            {1.8f, 1.2f, 0.0f},
            {},
            1.0f
        },
        // paddle
        my_gl::Physics<float>{
            {},
            {},
            1.0f
        },
        // walls
        my_gl::Physics<float>{
            {},
            {},
            1.0f
        },
    };

    // primitives
    std::array<my_gl::GeometryObjectPrimitive, PrimitiveIndex::COUNT> primitives = {
        // ball
        my_gl::GeometryObjectPrimitive{
            std::span<my_gl::TransformData>{transforms.begin(), 2},
            &physics[0],
            36,
            0,
            world_shader,
            vertex_arr_world,
            GL_TRIANGLES,
            my_gl::Material::EMERALD,
            my_gl::CollisionType::DEFAULT,
            nullptr,
        },
        // paddle
        my_gl::GeometryObjectPrimitive{
            std::span<my_gl::TransformData>{transforms.begin() + 2, 2},
            &physics[1],
            36,
            0,
            world_shader,
            vertex_arr_world,
            GL_TRIANGLES,
            my_gl::Material::RUBY,
            my_gl::CollisionType::STATIC,
            nullptr,
        },
        // left wall
        my_gl::GeometryObjectPrimitive{
            std::span<my_gl::TransformData>{transforms.begin() + 4, 2},
            &physics[1],
            36,
            0,
            world_shader,
            vertex_arr_world,
            GL_TRIANGLES,
            my_gl::Material::OBSIDIAN,
            my_gl::CollisionType::STATIC,
            nullptr,
        },
        // right wall
        my_gl::GeometryObjectPrimitive{
            std::span<my_gl::TransformData>{transforms.begin() + 6, 2},
            &physics[1],
            36,
            0,
            world_shader,
            vertex_arr_world,
            GL_TRIANGLES,
            my_gl::Material::OBSIDIAN,
            my_gl::CollisionType::STATIC,
            nullptr,
        },
        // top wall
        my_gl::GeometryObjectPrimitive{
            std::span<my_gl::TransformData>{transforms.begin() + 8, 2},
            &physics[1],
            36,
            0,
            world_shader,
            vertex_arr_world,
            GL_TRIANGLES,
            my_gl::Material::OBSIDIAN,
            my_gl::CollisionType::STATIC,
            nullptr,
        },
        // bottom wall
        my_gl::GeometryObjectPrimitive{
            std::span<my_gl::TransformData>{transforms.begin() + 10, 2},
            &physics[1],
            36,
            0,
            world_shader,
            vertex_arr_world,
            GL_TRIANGLES,
            my_gl::Material::OBSIDIAN,
            my_gl::CollisionType::STATIC,
            nullptr,
        },
        // light
        // my_gl::GeometryObjectPrimitive{
        //     std::span<my_gl::TransformData>{transforms.begin() + 6, 1},
        //     nullptr,
        //     36,
        //     0,
        //     light_shader,
        //     vertex_arr_light,
        //     GL_TRIANGLES,
        //     my_gl::Material::NO_MATERIAL,
        //     nullptr
        // }
    };

    // camera
    auto view_mat{ my_gl::globals::camera.get_view_mat() };
    auto proj_mat{ my_gl::math::Matrix44<float>::perspective_fov(
        my_gl::globals::camera.fov, my_gl::globals::camera.aspect, 0.1f, 50.0f
    )};

    my_gl::Renderer renderer{
        // std::span<my_gl::GeometryObjectComplex>{primitives}, //my_gl::create_cube_creature(world_shader, vertex_arr_world) },
        {},
        std::span<my_gl::GeometryObjectPrimitive>{primitives},
        std::move(view_mat),
        std::move(proj_mat),
    };

    world_shader.set_uniform_value("u_light.position",
        my_gl::globals::light.position
    );
    world_shader.set_uniform_value("u_light.ambient", my_gl::globals::light.ambient);
    world_shader.set_uniform_value("u_light.diffuse", my_gl::globals::light.diffuse);
    world_shader.set_uniform_value("u_light.specular", my_gl::globals::light.specular);

    world_shader.set_uniform_value("u_view_pos",
        // view coords
        // 0.0f, 0.0f, 0.0f
        // world coords
        my_gl::globals::camera.camera_pos
    );
    light_shader.set_uniform_value("u_color", 1.0f, 1.0f, 1.0f);

    // pass game state to GLFW
    auto game_state = GameState {
        .transforms{ std::span{transforms} },
        .physics{ std::span{physics} },
        .primitives{ std::span{primitives} }
    };
    my_gl::init_user_state(window.ptr, &game_state);

    bool is_rendering_started{false};
    my_gl::Duration_sec frame_duration{};
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window.ptr)) {
        auto frame_start{ std::chrono::steady_clock::now() };
        if (!is_rendering_started) {
            renderer._rendering_time_start = frame_start;
            is_rendering_started = true;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer._view_mat = my_gl::globals::camera.get_view_mat();
        renderer._proj_mat = my_gl::math::Matrix44<float>::perspective_fov(
            my_gl::globals::camera.fov, my_gl::globals::camera.aspect, 0.1f, 50.0f
        );

        world_shader.set_uniform_value("u_light.position",
            my_gl::globals::light.position
        );
        world_shader.set_uniform_value("u_view_pos",
            // view coords:
            // 0.0f, 0.0f, 0.0f
            // world coords:
            my_gl::globals::camera.camera_pos
        );

        float time_0to1 = my_gl::math::Global::map_duration_to01(renderer.get_curr_rendering_duration());
        renderer.render(frame_duration, time_0to1);

        glfwSwapBuffers(window.ptr);
        glfwPollEvents();

        frame_duration = std::chrono::steady_clock::now() - frame_start;
        renderer.update_time(frame_duration);
        my_gl::globals::delta_time = frame_duration.count();
    }

    return 0;
}

#pragma once
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <cstdint>
#include <string_view>
#include "geometryObject.hpp"
#include "matrix.hpp"
#include "sharedTypes.hpp"
#include "meshes.hpp"

namespace my_gl {
    class VertexArray;
    class Program;
    class Renderer;

    struct Attribute {
        const char*     name;
        int32_t         location{ -1 };
        GLenum          gl_type;
        uint16_t        count;
        uint16_t        byte_stride;
        uint16_t        byte_offset;
    };

    struct Uniform {
        const char*     name;
        int32_t         location{ -1 };
    };

    class VertexArray {
    public:
        VertexArray(
            meshes::Mesh&& mesh,
            const Program& program
        );
        VertexArray(
            const meshes::Mesh& mesh,
            const Program& program
        );
        VertexArray(
            meshes::Mesh&& mesh,
            const std::vector<const Program*>& programs
        );
        VertexArray(
            const meshes::Mesh& mesh,
            const std::vector<const Program*>& programs
        );
        VertexArray(
            const std::vector<meshes::Mesh>& meshes,
            const std::vector<const Program*>& programs
        );
        VertexArray(const VertexArray& rhs) = default;
        VertexArray(VertexArray&& rhs) = default;
        ~VertexArray();

        void bind() const { glBindVertexArray(_vao_id); }
        void un_bind() const { glBindVertexArray(0); }
        std::size_t get_ibo_size() const { return _ibo_data.size(); }
        const uint16_t* get_ibo_data() const { return _ibo_data.data(); }

    private:
        void init(const std::vector<const Program*>& programs);
        void init(const Program& program);
        void combine_meshes(const std::vector<meshes::Mesh>& meshes);

        std::vector<float>              _vbo_data;
        std::vector<uint16_t>           _ibo_data;
        uint32_t                        _vao_id;
        uint32_t                        _vbo_id;
        uint32_t                        _ibo_id;
    };

    class Program {
    public:
        Program(
            const char*                     vertex_shader_path,
            const char*                     fragment_shader_path,
            std::vector<Attribute>&&        attribs
        );
        // if also passing uniforms
        Program(
            const char*                     vertex_shader_path,
            const char*                     fragment_shader_path,
            std::vector<Attribute>&&        attribs,
            std::vector<Uniform>&&          uniforms
        );
        Program(const Program& rhs) = default;
        Program(Program&& rhs) = default;
        ~Program();

        const Attribute* const get_attrib(std::string_view attrib_name) const;
        const Uniform* const get_uniform(std::string_view unif_name) const;
        void  set_attrib(Attribute& attr);
        void  set_uniform_location(Uniform& unif);
        void  set_uniform_value(std::string_view unif_name, int32_t val) const;
        void  set_uniform_value(std::string_view unif_name, float val) const;
        void  set_uniform_value(std::string_view unif_name, float val1, float val2, float val3) const;
        void  set_uniform_value(std::string_view unif_name, const float* matrix_val) const;
        const std::unordered_map<std::string_view, Attribute>& get_attrs() const;
        const std::unordered_map<std::string_view, Uniform>& get_unifs() const;
        void  use() const { glUseProgram(_program_id); }
        void  un_use() const { glUseProgram(0); }
        uint32_t get_id() const { return _program_id; }

    private:
        std::unordered_map<std::string_view, Attribute>         _attrs;
        std::unordered_map<std::string_view, Uniform>           _unifs;
        uint32_t                                                _program_id{ 0 };
    };

    class Renderer {
    public:
        Renderer(
            std::vector<my_gl::GeometryObjectComplex>&&         complex_objs,
            std::vector<my_gl::GeometryObjectPrimitive>&&       primitives,
            math::Matrix44<float>&&                             view_mat,
            math::Matrix44<float>&&                             proj_mat
        );

        void render(float time_0to1);
        void update_time(Duration_sec frame_time);
        Duration_sec get_curr_rendering_duration() const;

        std::vector<my_gl::GeometryObjectComplex>           _complex_objs;
        std::vector<my_gl::GeometryObjectPrimitive>         _primitives;
        math::Matrix44<float>                               _view_mat;
        math::Matrix44<float>                               _proj_mat;
        Timepoint_sec                                       _rendering_time_curr;
        Timepoint_sec                                       _rendering_time_start;
    };
}

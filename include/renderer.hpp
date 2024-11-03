#pragma once
#include <unordered_map>
#include <vector>
#include <GLEW/glew.h>
#include <cstdint>
#include "matrix.hpp"

namespace my_gl {
    class VertexArray;
    class Program;
    class Renderer;
    class IGeometry_object;

    struct Attribute {
        const char*     name;
        int32_t         location{ -1 };
        GLenum          gl_type;
        uint16_t        count;
        uint16_t        byte_stride;
        uint16_t        byte_offset;
    };

    struct Uniform {
        const char*         name;
        int32_t             location{ -1 };
    };

    class VertexArray {
    public:
        VertexArray(
            std::vector<float>&& vbo_data,
            std::vector<uint16_t>&& ibo_data,
            const Program& program
        );
        ~VertexArray();

        void bind() const { glBindVertexArray(_vao_id); }
        void un_bind() const { glBindVertexArray(0); }
        std::size_t get_ibo_size() const { return _ibo_data.size(); }
        const uint16_t* get_ibo_data() const { return _ibo_data.data(); }

    private:
        std::vector<float>          _vbo_data;
        std::vector<uint16_t>       _ibo_data;
        const Program&              _program;
        uint32_t                    _vao_id;
        uint32_t                    _vbo_id;
        uint32_t                    _ibo_id;
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
        ~Program();

        const Attribute* const get_attrib(const char* attrib_name) const;
        const Uniform* const get_uniform(const char* unif_name) const;
        void  set_attrib(Attribute& attr);
        void  set_uniform(Uniform& unif);
        const std::unordered_map<const char*, Attribute>& get_attrs() const; 
        const std::unordered_map<const char*, Uniform>& get_unifs() const; 
        void  use() const { glUseProgram(_program_id); }
        void  un_use() const { glUseProgram(0); }
        uint32_t get_id() const { return _program_id; }

    private:
        std::unordered_map<const char*, Attribute>  _attrs;
        std::unordered_map<const char*, Uniform>    _unifs;
        uint32_t                                    _program_id{ 0 };
    };

    class Renderer {
    public:
        Renderer(
            std::vector<IGeometry_object*>&& objects,
            my_gl_math::Matrix44<float>&&   proj_mat,
            my_gl_math::Matrix44<float>&&   view_mat,
            my_gl::Program& program,
            my_gl::VertexArray& vertex_array
        );

        void render() const;
        void render(float curr_time) const;
        void set_proj_mat(my_gl_math::Matrix44<float>&& proj_mat);
        void set_view_mat(my_gl_math::Matrix44<float>&& view_mat);

    private:
        std::vector<IGeometry_object*>   _objects;
        my_gl_math::Matrix44<float>     _proj_mat;
        my_gl_math::Matrix44<float>     _view_mat;
        my_gl::Program&                 _program;
        my_gl::VertexArray&             _vertex_arr;
    };
}
#pragma once
#include <unordered_map>
#include <vector>
#include <GLEW/glew.h>

namespace my_gl {
    class Program;

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

    class VertexData {
    public:
        VertexData(
            std::vector<float>&& vbo_data,
            std::vector<uint16_t>&& ibo_data
        );
        ~VertexData();

        void init(const Program* program);
        void bind() const { glBindVertexArray(_vao_id); }
        void un_bind() const { glBindVertexArray(0); }
        std::size_t get_ibo_size() const { return _ibo_data.size(); }
        const uint16_t* get_ibo_data() const { return _ibo_data.data(); }

    private:
        std::vector<float>          _vbo_data;
        std::vector<uint16_t>       _ibo_data;
        const Program*              _program{ nullptr };
        uint32_t                    _vao_id;
        uint32_t                    _vbo_id;
        uint32_t                    _ibo_id;
    };

    class Program {
    public:
        Program(
            const char*                     vertex_shader_path,
            const char*                     fragment_shader_path,
            std::vector<Attribute>&&        attribs,
            // forwarded to VertexData
            std::vector<float>&&            vbo_data,
            std::vector<uint16_t>&&         ibo_data
        );
        // if also passing uniforms
        Program(
            const char*                     vertex_shader_path,
            const char*                     fragment_shader_path,
            std::vector<Attribute>&&        attribs,
            std::vector<Uniform>&&          uniforms,
            // forwarded to VertexData
            std::vector<float>&&            vbo_data,
            std::vector<uint16_t>&&         ibo_data
        );
        ~Program();

        const Attribute* const get_attrib(const char* attrib_name) const;
        const Uniform* const get_uniform(const char* unif_name) const;
        void  set_attrib(Attribute& attr);
        void  set_uniform(Uniform& unif);
        const std::unordered_map<const char*, Attribute>& get_attrs() const; 
        const std::unordered_map<const char*, Uniform>& get_unifs() const; 
        void  use() const { glUseProgram(_prog_id); }
        void  un_use() const { glUseProgram(0); }
        std::size_t get_vertex_count() const { return _vertex_data.get_ibo_size(); }
        uint32_t& get_prog_id() { return _prog_id; }
        const uint16_t* get_ibo_data() const { return _vertex_data.get_ibo_data(); }
        void  bind_vao() const { _vertex_data.bind(); }
        void  un_bind_vao() const { _vertex_data.un_bind(); }

    private:
        VertexData                                  _vertex_data;
        std::unordered_map<const char*, Attribute>  _attrs;
        std::unordered_map<const char*, Uniform>    _unifs;
        uint32_t                                    _prog_id{ 0 };
        
        void init() { _vertex_data.init(this); }
    };
}
#pragma once
#include <stdint.h>
#include <STB_IMG/stb_image.h>
#include <GLEW/glew.h>

namespace my_gl {
    class Program;
    class Uniform;

    class Texture {
    public:
        Texture(
            const char* path,
            const Program& program,
            const Uniform* const sampler_uniform,
            uint32_t sampler_uniform_value,
            GLenum texture_unit,
            bool is_3d = false,
            GLenum wrap_option = GL_REPEAT,
            GLenum min_filter_option = GL_LINEAR_MIPMAP_LINEAR,
            GLenum mag_filter_option = GL_LINEAR
        );
        //~Texture();
        void        bind() const;
        void        un_bind() const;
        uint32_t    id() const { return _id; }
        uint8_t*    data() const { return _data; }
        int         width() const { return _width; }
        int         height() const { return _height; }
        int         depth() const { return _depth; }
        int         color_channels() const { return _color_channels; }

    private:
        uint8_t*    _data;
        uint32_t    _id;
        GLenum      _texture_unit;
        int         _width;
        int         _height;
        int         _depth;
        int         _color_channels;
        bool        _3d;
    };
}
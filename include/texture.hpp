#pragma once
#include <stdint.h>
#include <STB_IMG/stb_image.h>
#include <GL/glew.h>

namespace my_gl {
    class Program;
    struct Uniform;

    struct Texture {
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
        Texture(const Texture& rhs) = default;
        Texture(Texture&& rhs) = default;
        Texture& operator=(const Texture& rhs) = default;
        Texture& operator=(Texture&& rhs) = default;

        void        bind() const;
        void        un_bind() const;

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

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "texture.hpp"
#include "renderer.hpp"

namespace my_gl {
    Texture::Texture(
        const char* path,
        const Program& program,
        const Uniform* const sampler_uniform,
        uint32_t sampler_uniform_value,
        GLenum texture_unit,
        bool is_3d,
        GLenum wrap_option,
        GLenum min_filter_option,
        GLenum mag_filter_option
    )
        : _3d{ is_3d }
        , _texture_unit{ texture_unit }
    {
        glGenTextures(1, &_id);

        // set texture unit
        program.use();
        glUniform1i(sampler_uniform->location, sampler_uniform_value);

        bind();

        // set options
        if (!_3d) {
            //glBindTexture(GL_TEXTURE_2D, _id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_option);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_option);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_option);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_option);
        }
        else {
            //glBindTexture(GL_TEXTURE_3D, _id);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap_option);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap_option);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, min_filter_option);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, mag_filter_option);
        }

        _data = stbi_load(path, &_width, &_height, &_color_channels, 0);
        
        if (_data) {
            // 2d texture
            if (!_3d) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            // 3d texture
            else {
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, _width, _height, _depth, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
                glGenerateMipmap(GL_TEXTURE_3D);
            }

            stbi_image_free(_data);
        }
        else {
            std::cerr << "failed to load texture from path: " << path << '\n';
        }
    }

    void Texture::bind() const {
        glActiveTexture(_texture_unit);
        if (!_3d) {
            glBindTexture(GL_TEXTURE_2D, _id);
        }
        else {
            glBindTexture(GL_TEXTURE_3D, _id);
        }
    }

    void Texture::un_bind() const {
        if (!_3d) {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else {
            glBindTexture(GL_TEXTURE_3D, 0);
        }
    }
}
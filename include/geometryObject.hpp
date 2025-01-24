#pragma once
#include <GLEW/glew.h>
#include <cstddef>
#include <vector>
#include <unordered_map>
#include <optional>
#include "animation.hpp"
#include "matrix.hpp"
#include "texture.hpp"
#include "sharedTypes.hpp"

namespace my_gl {
    class Program;
    class VertexArray;
    class ObjectCache;

    class GeometryObject : public IRenderable {
    public:
        GeometryObject(
            std::vector<my_gl_math::Matrix44<float>>&& transforms,
            std::vector<my_gl::Animation<float>>&&     animations,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset,
            const Program&                             program,
            const VertexArray&                         vao,
            GLenum                                     draw_type,
            std::vector<const my_gl::Texture*>&&       textures
        );
        GeometryObject(
            std::vector<my_gl::Animation<float>>&&     animations,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset,
            const Program&                             program,
            const VertexArray&                         vao,
            GLenum                                     draw_type,
            std::vector<const my_gl::Texture*>&&       textures
        );
        GeometryObject(
            std::vector<my_gl_math::Matrix44<float>>&& transforms,
            std::size_t                                vertices_count,
            std::size_t                                buffer_byte_offset,
            const Program&                             program,
            const VertexArray&                         vao,
            GLenum                                     draw_type,
            std::vector<const my_gl::Texture*>&&       textures
        );

        GeometryObject(const GeometryObject& rhs) = default;
        GeometryObject(GeometryObject&& rhs) = default;
        GeometryObject& operator=(const GeometryObject& rhs) = delete;
        GeometryObject& operator=(GeometryObject&& rhs) = delete;
        ~GeometryObject() = default;

        my_gl_math::Matrix44<float> get_local_mat() const;
        void                        bind_state() const;
        void                        un_bind_state() const;
        void                        draw() const;
        void                        update_anims_time(Duration_sec frame_time) const override;
        void                        render(const my_gl_math::Matrix44<float>& world_matrix, float time_0to1) const override;

        constexpr std::size_t       get_vertices_count() const { 
            return _vertices_count;
        }
        constexpr std::size_t       get_buffer_byte_offset() const {
            return _buffer_byte_offset;
        }
        const Program&              get_program() const { return _program; }
        const VertexArray&          get_vao() const { return _vao; }

    private:
        std::vector<my_gl_math::Matrix44<float>>            _transforms;
        // "mutable" so we can mark functions as const
        mutable std::vector<my_gl::Animation<float>>        _animations;
        std::vector<const my_gl::Texture*>                  _textures;
        std::size_t                                         _vertices_count;
        std::size_t                                         _buffer_byte_offset;
        std::size_t                                         _uid;
        const Program&                                      _program;
        const VertexArray&                                  _vao;
        GLenum                                              _draw_type;
    };

    class IdGenerator {
    public:
        static std::size_t gen();
        static std::size_t peek();
        static void        reset();

    private:
        static inline std::size_t _uid;
    };

// TODO:
// research: https://gameprogrammingpatterns.com/object-pool.html
    /*class ObjectPool {*/

    struct CacheItem {
        my_gl_math::Matrix44<float> contents;
        // flag set at first to do initial calculation
        bool                        was_modified{ true };
    };

    class ObjectCache {
    public:
        void                                        add_item(std::size_t obj_id, my_gl_math::Matrix44<float> transform_res);
        std::optional<my_gl_math::Matrix44<float>>  get_item(std::size_t obj_id) const;
        bool                                        toggle_modified_state(std::size_t obj_id);
        bool                                        should_recalc(std::size_t obj_id) const;
    private:
        std::unordered_map<std::size_t, CacheItem> _map;
    };
}

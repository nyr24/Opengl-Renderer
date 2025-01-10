#include "geometryObject.hpp"
#include "matrix.hpp"
#include "renderer.hpp"
#include <optional>

my_gl::GeometryObject::GeometryObject(
    std::vector<my_gl_math::Matrix44<float>>&& transforms,
    std::vector<my_gl::Animation<float>>&&     animations,
    std::size_t                                vertices_count,
    std::size_t                                buffer_byte_offset,
    const Program&                             program,
    const VertexArray&                         vao,
    GLenum                                     draw_type,
    std::vector<const my_gl::Texture*>&&       textures = {}
)
    : _transforms{ std::move(transforms) }
    , _animations{ std::move(animations) }
    , _textures{ std::move(textures) }
    , _vertices_count{ vertices_count }
    , _buffer_byte_offset{ buffer_byte_offset }
    , _uid{ IdGenerator::gen() }
    , _program{ program }
    , _vao{ vao }
    , _draw_type{ draw_type }
{}

my_gl::GeometryObject::GeometryObject(
    std::vector<my_gl::Animation<float>>&&     animations,
    std::size_t                                vertices_count,
    std::size_t                                buffer_byte_offset,
    const Program&                             program,
    const VertexArray&                         vao,
    GLenum                                     draw_type,
    std::vector<const my_gl::Texture*>&&       textures = {}
)
    : _animations{ std::move(animations) }
    , _vertices_count{ vertices_count }
    , _textures{ std::move(textures) }
    , _buffer_byte_offset{ buffer_byte_offset }
    , _uid{ IdGenerator::gen() }
    , _program{ program } 
    , _vao{ vao }
    , _draw_type{ draw_type }
{}

my_gl::GeometryObject::GeometryObject(
    std::vector<my_gl_math::Matrix44<float>>&& transforms,
    std::size_t                                vertices_count,
    std::size_t                                buffer_byte_offset,
    const Program&                             program,
    const VertexArray&                         vao,
    GLenum                                     draw_type,
    std::vector<const my_gl::Texture*>&&       textures = {}
)
    : _transforms{ std::move(transforms) }
    , _vertices_count{ vertices_count }
    , _textures{ std::move(textures) }
    , _buffer_byte_offset{ buffer_byte_offset }
    , _uid{ IdGenerator::gen() }
    , _program{ program }
    , _vao{ vao }
    , _draw_type{ draw_type }
{}

my_gl_math::Matrix44<float> my_gl::GeometryObject::get_local_mat(ObjectCache obj_cache) const {
    auto result_mat{ my_gl_math::Matrix44<float>::identity() };

    if (_transforms.size() > 0) {
        // do calc if was modified or not in the cache
        if (obj_cache.should_recalc(_uid)) {
            for (const auto& curr_mat : _transforms) {
                result_mat *= curr_mat;
            }
            obj_cache.add_item(_uid, result_mat);
        }
        else {
            result_mat = obj_cache.get_item(_uid).value();
        }
    }

    if (_animations.size() > 0) {
        for (auto& curr_anim : _animations) {
            result_mat *= curr_anim.update();
        }
    }

    return result_mat;
}

void my_gl::GeometryObject::update_anims_time(Duration_sec frame_time) const {
    if (_animations.size() > 0) {
        for (auto& anim : _animations) {
            anim.update_time(frame_time);
        }
    }
}

void my_gl::GeometryObject::bind_state() const {
    if (_textures.size() > 0) {
        for (const auto* texture : _textures) {
            texture->bind();
        }
    }
    _program.use();
    _vao.bind();
}

void my_gl::GeometryObject::un_bind_state() const {
    if (_textures.size() > 0) {
        for (const auto* texture : _textures) {
            texture->un_bind();
        }
    }
    _program.un_use();
    _vao.un_bind();
}

void my_gl::GeometryObject::draw() const {
    glDrawElements(
        _draw_type,
        _vertices_count,
        GL_UNSIGNED_SHORT,
        reinterpret_cast<const void*>(_buffer_byte_offset)
    );
}

std::size_t my_gl::IdGenerator::gen() {
    return _uid++;
}

std::size_t my_gl::IdGenerator::peek() {
    return _uid;
}

void my_gl::IdGenerator::reset() {
    _uid = 0;
}

void my_gl::ObjectCache::add_item(std::size_t obj_id, my_gl_math::Matrix44<float> transform_res) {
    if (_map.contains(obj_id)) {
        return;
    }

    _map[obj_id] = my_gl::CacheItem{ .contents = transform_res };
}

std::optional<my_gl_math::Matrix44<float>> my_gl::ObjectCache::get_item(std::size_t obj_id) const {
    if (_map.contains(obj_id)) {
        return { _map.at(obj_id).contents };
    } else {
        return std::nullopt;
    }
}

bool my_gl::ObjectCache::toggle_modified_state(std::size_t obj_id) {
    if (!_map.contains(obj_id)) {
        return false;
    }

    auto& item{ _map.at(obj_id) };

    item.was_modified = !item.was_modified;
    return true;
}

bool my_gl::ObjectCache::should_recalc(std::size_t obj_id) const {
    if (!_map.contains(obj_id)) {
        return false;
    }

    return _map.at(obj_id).was_modified;
}

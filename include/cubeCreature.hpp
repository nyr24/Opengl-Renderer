#pragma once

#include "geometryObject.hpp"
#include "renderer.hpp"
#include "sharedTypes.hpp"

namespace my_gl {
    struct CubeCreatureParams;
    class IRenderable;

    class CubeCreature : public IRenderable {
    public:
        static CubeCreature create(const Program& program, const VertexArray& vertex_array);

        void                        update_anims_time(Duration_sec frame_time) const override;
        void                        render(const my_gl_math::Matrix44<float>& world_matrix, float time_0to1) const override;
    private:
        GeometryObject _head;
        GeometryObject _body;
        GeometryObject _left_hand;
        GeometryObject _right_hand;
        GeometryObject _left_leg;
        GeometryObject _right_leg;

        CubeCreature(CubeCreatureParams&& params);
    };

    struct CubeCreatureParams {
        GeometryObject&& head;
        GeometryObject&& body;
        GeometryObject&& left_hand;
        GeometryObject&& right_hand;
        GeometryObject&& left_leg;
        GeometryObject&& right_leg;
    };
}

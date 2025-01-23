#pragma once

#include "geometryObject.hpp"
#include "renderer.hpp"
#include <initializer_list>

namespace my_gl {
    class CubeCreature {
    public:
    private:
        GeometryObject _head;
        GeometryObject _body;
        GeometryObject _left_hand;
        GeometryObject _right_hand;
        GeometryObject _left_leg;
        GeometryObject _right_leg;

        CubeCreature(
            GeometryObject&& head,
            GeometryObject&& body,
            GeometryObject&& left_hand,
            GeometryObject&& right_hand,
            GeometryObject&& left_leg,
            GeometryObject&& right_leg
        );

        friend CubeCreature init(const Program& program, const VertexArray& vertex_array);
    };
}

//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#pragma once

#include <vector>

#include <qtil/vector_util.hpp>

#include "./Stroke.hpp"

namespace quartz::core {

    class Drawing {
        ::std::vector<Stroke> strokes_;
        ::std::vector<Point> points_;
        AnimFile* file_;
        DrawingId id_;

    public:

        Drawing(IdKey, AnimFile*, DrawingId);

        void add_stroke(Color color, num_t thickness, CornerType corner_type = CornerType::Round, num_t miter_limit = 10.0);
        Stroke& get_stroke(size_t index);

        void add_point(const Point& point);

        ::qtil::VecIterable<Point> points() { return ::qtil::VecIterable{points_}; }
        ::qtil::VecIterable<Stroke> strokes() { return ::qtil::VecIterable{strokes_}; }

        Point& operator[](size_t index);
        const Point& operator[](size_t index) const;
    };
}

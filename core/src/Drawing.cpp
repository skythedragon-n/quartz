//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include "Drawing.hpp"

namespace quartz::core {
    Drawing::Drawing(IdKey, AnimFile* file, DrawingId id) :
    file_(file),
    id_(id)
    {}

    void Drawing::add_stroke(Color color, num_t thickness, CornerType corner_type, num_t miter_limit) {
        strokes_.emplace_back(file_, id_, thickness, color, corner_type, miter_limit);
    }

    void Drawing::add_point(const Point& point) {
        points_.emplace_back(point);
    }

    Stroke& Drawing::get_stroke(size_t index) {
        return strokes_[index];
    }

    Point& Drawing::operator[](size_t index) {
        return points_[index];
    }

    const Point& Drawing::operator[](size_t index) const {
        return points_[index];
    }
}
